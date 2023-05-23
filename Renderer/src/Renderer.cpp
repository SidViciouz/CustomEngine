#include "Renderer.h"
#include <iostream>
#include <vector>
#include "ResourceManager.h"
#include "FrameData.h"
#include "Util.h"

namespace Renderer
{
	CRenderer::CRenderer(HWND pWindowHandle)
		: mWindowHandle{ pWindowHandle }
	{
		RECT lRect;
		GetWindowRect(pWindowHandle, &lRect);
		mWidth = lRect.right - lRect.left;
		mHeight = lRect.bottom - lRect.top;

		printf("width : %d, height ; %d\n", mWidth, mHeight);
	}

	CRenderer::~CRenderer()
	{

	}

	void CRenderer::Initialize()
	{
		EnableDebug();
		CreateDevice();
		CreateCommandObjects();
		CreateResourceManager();
		CreateFrameData();
		CreateSwapchain();
		CreateDepthBuffer();
	}



	void CRenderer::EnableDebug()
	{
		
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&mDebugController))))
		{
			mDebugController->EnableDebugLayer();
			mDebugEnabled = true;
		}
		else
		{
			throw string("enabling debug fails.");
		}

	}

	void CRenderer::CreateDevice()
	{
		if (!SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(mFactory.GetAddressOf()))))
			throw string("creating dxgi factory1 fails.");
		
		InitAdaptInfo();

		if (!SUCCEEDED(D3D12CreateDevice(mAdapters[mCurrentAdapter], D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mDevice))))
			throw string("creating device fails.");

	}

	void CRenderer::InitAdaptInfo()
	{
		mAdapterNum = 0;
		while (mFactory->EnumAdapters(mAdapterNum, &mAdapters[mAdapterNum]) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC lDesc;
			mAdapters[mAdapterNum]->GetDesc(&lDesc);
			memcpy(mAdapterNames[mAdapterNum], lDesc.Description, sizeof(wchar_t) * 128);
			++mAdapterNum;
		}
		for (int i = 0; i < mAdapterNum; ++i)
			wcout << mAdapterNames[i] << endl;
	}

	void CRenderer::CreateCommandObjects()
	{
		if(!SUCCEEDED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()))))
			throw string("creating command allocator fails.");

		D3D12_COMMAND_QUEUE_DESC lDesc = {};
		lDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		lDesc.NodeMask = 0;
		lDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		lDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

		if (!SUCCEEDED(mDevice->CreateCommandQueue(&lDesc,IID_PPV_ARGS(mCommandQueue.GetAddressOf()))))
			throw string("creating command queue fails.");
		
		if (!SUCCEEDED(mDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()))))
			throw string("creating command list fails");
	}

	void CRenderer::CreateResourceManager()
	{
		mResourceManager = make_unique<CResourceManager>(mDevice.Get(),mCommandList.Get());
	}

	void CRenderer::CreateFrameData()
	{
		mFrameData = make_unique<CFrameData>(mDevice.Get(),mFrameNum);
		for (int lFrameIndex = 0; lFrameIndex < mFrameNum; ++lFrameIndex)
		{
			mFrameData->SetObjectConstantBufferHandle(
				lFrameIndex, mResourceManager->CreateBuffer(Math::Alignment(sizeof(SObjectData)*MAX_OBJECT_NUM , 256), EResourceHeapType::eUpload));
			mFrameData->SetWorldConstantBufferHandle(
				lFrameIndex, mResourceManager->CreateBuffer(Math::Alignment(sizeof(SWorldData), 256), EResourceHeapType::eUpload));
		}
	}

	void CRenderer::CreateSwapchain()
	{
		DXGI_SWAP_CHAIN_DESC lSwapchainDesc = {};
		
		//swapchain width and height should change depending on window size.
		//window object must be passed to CRenderer class.
		lSwapchainDesc.BufferCount = SWAPCHAIN_BUFFER_NUM;
		lSwapchainDesc.BufferDesc.Width = mWidth;
		lSwapchainDesc.BufferDesc.Height = mHeight;
		lSwapchainDesc.BufferDesc.Format = mBackBufferFormat;
		lSwapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
		lSwapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
		lSwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		lSwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		lSwapchainDesc.SampleDesc.Count = 1;
		lSwapchainDesc.SampleDesc.Quality = 0;
		lSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		lSwapchainDesc.Windowed = true;
		lSwapchainDesc.OutputWindow = mWindowHandle;
		lSwapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		lSwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		if (!SUCCEEDED(mFactory->CreateSwapChain(mCommandQueue.Get(), &lSwapchainDesc, mSwapchain.GetAddressOf())))
			throw string("creating swapchain fails.");

		for (int lSwapChainIndex = 0; lSwapChainIndex < SWAPCHAIN_BUFFER_NUM; ++lSwapChainIndex)
		{
			//store buffer in resource manager and get its handle
			mSwapchainBufferHandle[lSwapChainIndex] = mResourceManager->StoreSwapchainBuffer(mSwapchain.Get(), lSwapChainIndex);

			//create RTV for a swapchain buffer
			mRtvHandle[lSwapChainIndex] =  mResourceManager->CreateDescriptor(mSwapchainBufferHandle[lSwapChainIndex], EDescriptorType::eRTV);
		}
	}

	void CRenderer::CreateDepthBuffer()
	{
		mDepthBufferHandle = mResourceManager->CreateTexture2D(mWidth, mHeight, 1, mDepthBufferFormat, EResourceHeapType::eDefault, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		mResourceManager->ChangeState(mDepthBufferHandle, D3D12_RESOURCE_STATE_DEPTH_READ);

		mDsvHandle = mResourceManager->CreateDescriptor(mDepthBufferHandle, EDescriptorType::eDSV);
	}

}
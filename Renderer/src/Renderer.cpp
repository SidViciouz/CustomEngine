#include "Renderer.h"
#include <iostream>
#include <vector>
#include "ResourceManager.h"
#include "FrameData.h"
#include "Util.h"

namespace Renderer
{
	CRenderer::CRenderer()
	{

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
	}

	void CRenderer::Run()
	{
		while (1);
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
		mResourceManager = make_unique<CResourceManager>(mDevice.Get());
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
}
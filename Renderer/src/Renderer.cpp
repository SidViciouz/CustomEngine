#include "Renderer.h"
#include "ResourceManager.h"
#include "FrameData.h"
#include "Util.h"
#include <Vector3.h>
#include "Camera.h"
#include "Mesh.h"
#include "Object.h"
#include "d3dx12.h"

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
		CreateFence();
		CreateCommandObjects();
		CreateResourceManager();
		CreateFrameData();
		CreateSwapchain();
		CreateDepthBuffer();
		SetViewportAndScissor();
		LoadShaders();
		CreateRootSignatures();
		CreatePSO();

		if(!SUCCEEDED(mCommandList->Close()))
			throw string("commandlist closing fails.");
		ID3D12CommandList* lists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(1, lists);
	}



	void CRenderer::Resize()
	{
		//resize window
		RECT lRect;
		GetWindowRect(mWindowHandle, &lRect);

		mWidth = lRect.right - lRect.left;
		mHeight = lRect.bottom - lRect.top;



		WaitUntilAllCommandDone();

		if (!SUCCEEDED(mCommandList->Reset(mCommandAllocator.Get(), nullptr)))
			throw string("reset command list fails.");


		//release back buffer and depth buffer
		for (int lBackBufferIndex = 0; lBackBufferIndex < mSwapchainBufferNum; ++lBackBufferIndex)
		{
			mResourceManager->ReleaseResource(mSwapchainBufferHandle[lBackBufferIndex]);
		}

		mResourceManager->ReleaseResource(mDepthBufferHandle);

		//resize swapchain
		mSwapchain->ResizeBuffers(mSwapchainBufferNum, mWidth, mHeight, mBackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		mCurrentBackBuffer = 0;


		//store back buffer
		for (int lBackBufferIndex = 0; lBackBufferIndex < SWAPCHAIN_BUFFER_NUM; ++lBackBufferIndex)
		{
			//store buffer in resource manager and get its handle
			mResourceManager->StoreSwapchainBuffer(mSwapchain.Get(), lBackBufferIndex, mSwapchainBufferHandle[lBackBufferIndex]);

			//recreate RTV for a swapchain buffer
			mResourceManager->RecreateDescriptor(mSwapchainBufferHandle[lBackBufferIndex], EDescriptorType::eRTV, mRtvHandle[lBackBufferIndex]);
		}

		//recreate depth buffer
		mDepthBufferHandle = mResourceManager->CreateDepthTexture(mWidth, mHeight, 1, mDepthBufferFormat, EResourceHeapType::eDefault, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		mResourceManager->ChangeState(mDepthBufferHandle, D3D12_RESOURCE_STATE_DEPTH_WRITE);

		mResourceManager->RecreateDescriptor(mDepthBufferHandle, EDescriptorType::eDSV, mDsvHandle);


		//reset viewport and scissor rect
		SetViewportAndScissor();


		if (!SUCCEEDED(mCommandList->Close()))
			throw string("commandlist closing fails.");
		ID3D12CommandList* lists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(1, lists);

		WaitUntilAllCommandDone();
	}



	void CRenderer::SetCamera(shared_ptr<CCamera> pCamera)
	{
		mCamera = pCamera;
	}



	int	CRenderer::SetMesh(shared_ptr<CMesh> pMesh)
	{

		int lVertexCount = pMesh->GetVertexCount();
		const SVertex* lVertexDatas = pMesh->GetVertexDatas();

		//create vertex buffer
		int lResourceHandle = mResourceManager->CreateBuffer(sizeof(SVertex) * lVertexCount, EResourceHeapType::eUpload);
		mResourceManager->Upload(lResourceHandle, lVertexDatas, sizeof(SVertex) * lVertexCount, 1, 0, 0);

		pMesh->SetVertexBufferHandle(lResourceHandle);

		for (int lSubMeshIndex = 0; lSubMeshIndex < pMesh->GetSubMeshCount(); ++lSubMeshIndex)
		{
			int lIndexCount = pMesh->GetIndexCount(lSubMeshIndex);
			const uint16_t* lIndexDatas = pMesh->GetIndexDatas(lSubMeshIndex);
			
			//create Index buffer
			int lIndexHandle = mResourceManager->CreateBuffer(sizeof(uint16_t) * lIndexCount, EResourceHeapType::eUpload);
			mResourceManager->Upload(lIndexHandle, lIndexDatas, sizeof(uint16_t) * lIndexCount, 1, 0, 0);

			pMesh->SetIndexBufferHandle(lSubMeshIndex, lIndexHandle);
		}

		//create skeleton buffer
		if (pMesh->HasSkeleton())
		{
			for (int lFrameIndex = 0; lFrameIndex < mFrameNum; ++lFrameIndex)
			{
				mFrameData->SetSkeletonConstantBufferHandle(
					lFrameIndex, mMeshCount, mResourceManager->CreateBuffer(Math::Alignment(sizeof(Math::SMatrix4) * pMesh->GetBoneCount(), 256), EResourceHeapType::eUpload));
			}
		}

		mMeshes.push_back(pMesh);

		return mMeshCount++;
	}



	int CRenderer::SetObject(shared_ptr<CObject> pObject)
	{
		for (int lFrameIndex = 0; lFrameIndex < mFrameNum; ++lFrameIndex)
		{
			mFrameData->SetObjectConstantBufferHandle(
				lFrameIndex, mObjectCount, mResourceManager->CreateBuffer(Math::Alignment(sizeof(SObjectData), 256), EResourceHeapType::eUpload));
		}

		mObjects.push_back(pObject);

		return mObjectCount++;
	}


	void CRenderer::LoadBegin()
	{
		mCopyCommandAllocator->Reset();
		mCopyCommandList->Reset(mCopyCommandAllocator.Get(), nullptr);
	}



	void CRenderer::LoadEnd()
	{
		mCopyCommandList->Close();
		ID3D12CommandList* lists[] = { mCopyCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(1, lists);
	}



	int	CRenderer::LoadTexture(const wchar_t* pFilePath)
	{
		int lUploadHandle = mResourceManager->CreateNullResourcePointer();

		int lTextureHandle = mResourceManager->CreateNullResourcePointer();
		mResourceManager->LoadDDS(lTextureHandle, lUploadHandle, pFilePath);
;
		int lDescriptorHandle = mResourceManager->CreateDescriptor(lTextureHandle, EDescriptorType::eSRV);

		mUploadBufferHandleMap[lTextureHandle] = lUploadHandle;

		mDescriptorHandleMap[lTextureHandle] = lDescriptorHandle;

		return lTextureHandle;
	}



	void CRenderer::DrawBegin()
	{
		//wait until current frame is done with rendering.
		mCurrentFrame = (mCurrentFrame + 1) % mFrameNum;

		UINT64 pFenceValue = mFrameData->GetFenceValue(mCurrentFrame);

		if (pFenceValue != 0 && mFence->GetCompletedValue() < pFenceValue)
		{
			HANDLE event = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			mFence->SetEventOnCompletion(pFenceValue, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}


		//upload world data
		UploadWorldConstantBuffer();
		UploadObjectConstantBuffer();
		UploadSkeletonConstantBuffer();

		//reset command list
		mFrameData->GetCommandAllocator(mCurrentFrame)->Reset();
		mCommandList->Reset(mFrameData->GetCommandAllocator(mCurrentFrame), nullptr);

		mResourceManager->ChangeState(mSwapchainBufferHandle[mCurrentBackBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET);

		mCommandList->RSSetScissorRects(1, &mScissor);
		mCommandList->RSSetViewports(1, &mViewport);

		D3D12_CPU_DESCRIPTOR_HANDLE lRtvHandle = mResourceManager->GetCpuHandle(mRtvHandle[mCurrentBackBuffer]);
		D3D12_CPU_DESCRIPTOR_HANDLE lDsvHandle = mResourceManager->GetCpuHandle(mDsvHandle);
		float lRgba[4] = { 0.00f,0.00f,0.00f,1.0f };
		mCommandList->ClearRenderTargetView(lRtvHandle, lRgba, 0, nullptr);
		mCommandList->ClearDepthStencilView(lDsvHandle,D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		mCommandList->OMSetRenderTargets(1, &lRtvHandle, true, &lDsvHandle);
	}



	void CRenderer::DrawEnd()
	{
		mResourceManager->ChangeState(mSwapchainBufferHandle[mCurrentBackBuffer], D3D12_RESOURCE_STATE_PRESENT);

		mCommandList->Close();

		ID3D12CommandList* lists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(1, lists);

		if (!SUCCEEDED(mSwapchain->Present(0, 0)))
			throw string("swapchain present fails.");

		mCurrentBackBuffer = (mCurrentBackBuffer + 1) % SWAPCHAIN_BUFFER_NUM;
		mFrameData->SetFenceValue(mCurrentFrame,++mFenceValue);
		mCommandQueue->Signal(mFence.Get(), mFenceValue);
	}



	void CRenderer::DrawLine(void* pData)
	{
		mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		mCommandList->SetGraphicsRootSignature(mRootSignatures["Line"].Get());
		mCommandList->SetPipelineState(mPSOs["Line"].Get());
		mCommandList->SetGraphicsRoot32BitConstants(0, 9, pData, 0);
		mCommandList->SetGraphicsRootConstantBufferView(1, mResourceManager->GetGpuVirtualAddress(mFrameData->GetWorldConstantBufferHandle(mCurrentFrame)));
		mCommandList->IASetVertexBuffers(0, 0, nullptr);
		mCommandList->IASetIndexBuffer(nullptr);
		mCommandList->DrawInstanced(2,1,0,0);
	}



	void CRenderer::DrawMesh(int pMeshHandle, int pObjectHandle)
	{
		D3D12_VERTEX_BUFFER_VIEW lVertexBufferView;
		lVertexBufferView.BufferLocation = mResourceManager->GetGpuVirtualAddress(mMeshes[pMeshHandle]->GetVertexBufferHandle());
		lVertexBufferView.SizeInBytes = sizeof(SVertex) * mMeshes[pMeshHandle]->GetVertexCount();
		lVertexBufferView.StrideInBytes = sizeof(SVertex);

		mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mCommandList->SetGraphicsRootSignature(mRootSignatures["Default"].Get());
		mCommandList->SetPipelineState(mPSOs["Default"].Get());
		mCommandList->SetGraphicsRootConstantBufferView(0, mResourceManager->GetGpuVirtualAddress(mFrameData->GetObjectConstantBufferHandle(mCurrentFrame, pObjectHandle)));
		mCommandList->SetGraphicsRootConstantBufferView(1, mResourceManager->GetGpuVirtualAddress(mFrameData->GetWorldConstantBufferHandle(mCurrentFrame)));
		mCommandList->IASetVertexBuffers(0, 1, &lVertexBufferView);

		for (int lSubMeshIndex = 0; lSubMeshIndex < mMeshes[pMeshHandle]->GetSubMeshCount(); ++lSubMeshIndex)
		{
			D3D12_INDEX_BUFFER_VIEW lIndexBufferView;
			lIndexBufferView.BufferLocation = mResourceManager->GetGpuVirtualAddress(mMeshes[pMeshHandle]->GetIndexBufferHandle(lSubMeshIndex));
			lIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
			lIndexBufferView.SizeInBytes = sizeof(uint16_t) * mMeshes[pMeshHandle]->GetIndexCount(lSubMeshIndex);

			mCommandList->IASetIndexBuffer(&lIndexBufferView);
			mCommandList->DrawIndexedInstanced(mMeshes[pMeshHandle]->GetIndexCount(lSubMeshIndex), 1, 0, 0,0);
		}

	}

	void CRenderer::DrawMeshPBR(int pMeshHandle, int pObjectHandle,int pBaseColorTextureHandle, int pMetallicTextureHandle, int pNormalTextureHandle, int pRoughnessTextureHandle, int pAmbientOcculstionTextureHandle)
	{
		D3D12_VERTEX_BUFFER_VIEW lVertexBufferView;
		lVertexBufferView.BufferLocation = mResourceManager->GetGpuVirtualAddress(mMeshes[pMeshHandle]->GetVertexBufferHandle());
		lVertexBufferView.SizeInBytes = sizeof(SVertex) * mMeshes[pMeshHandle]->GetVertexCount();
		lVertexBufferView.StrideInBytes = sizeof(SVertex);

		SPBRData lPBRData = {};

		ID3D12DescriptorHeap* lDescriptorHeap = mResourceManager->GetDescriptorHeap(EDescriptorType::eSRV);
		mCommandList->SetDescriptorHeaps(1, &lDescriptorHeap);
		mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mCommandList->SetGraphicsRootSignature(mRootSignatures["PBR"].Get());
		mCommandList->SetPipelineState(mPSOs["PBR"].Get());
		mCommandList->SetGraphicsRootConstantBufferView(0, mResourceManager->GetGpuVirtualAddress(mFrameData->GetObjectConstantBufferHandle(mCurrentFrame, pObjectHandle)));

		mCommandList->SetGraphicsRootConstantBufferView(1, mResourceManager->GetGpuVirtualAddress(mFrameData->GetWorldConstantBufferHandle(mCurrentFrame)));

		if (pBaseColorTextureHandle > -1)
		{
			mCommandList->SetGraphicsRootDescriptorTable(2, mResourceManager->GetGpuHandle(mDescriptorHandleMap[pBaseColorTextureHandle]));
			lPBRData.mHasAlbedoMap = 1;
		}
		else
		{
			lPBRData.mHasAlbedoMap = 0;
			lPBRData.mAlbedo[0] = 0.2;
			lPBRData.mAlbedo[1] = 0.2;
			lPBRData.mAlbedo[2] = 1.0;
		}

		if (pMetallicTextureHandle > -1)
		{
			mCommandList->SetGraphicsRootDescriptorTable(3, mResourceManager->GetGpuHandle(mDescriptorHandleMap[pMetallicTextureHandle]));
			lPBRData.mHasMetallicMap = 1;
		}
		else
		{
			lPBRData.mHasMetallicMap = 0;
			lPBRData.mMetallic = 0.0f;
		}

		if (pNormalTextureHandle > -1)
		{
			mCommandList->SetGraphicsRootDescriptorTable(4, mResourceManager->GetGpuHandle(mDescriptorHandleMap[pNormalTextureHandle]));
			lPBRData.mHasNormalMap = 1;
		}
		else
		{
			lPBRData.mHasNormalMap = 0;
		}

		if (pRoughnessTextureHandle > -1)
		{
			mCommandList->SetGraphicsRootDescriptorTable(5, mResourceManager->GetGpuHandle(mDescriptorHandleMap[pRoughnessTextureHandle]));
			lPBRData.mHasRoughnessMap = 1;
		}
		else
		{
			lPBRData.mHasRoughnessMap = 0;
			lPBRData.mRoughness = 1.0f;
		}

		if (pAmbientOcculstionTextureHandle > -1)
		{
			mCommandList->SetGraphicsRootDescriptorTable(6, mResourceManager->GetGpuHandle(mDescriptorHandleMap[pAmbientOcculstionTextureHandle]));
			lPBRData.mHasAOMap = 1;
		}
		else
		{
			lPBRData.mHasAOMap = 0;
		}

		if (mMeshes[pMeshHandle]->HasSkeleton())
		{
			mCommandList->SetGraphicsRootConstantBufferView(8, mResourceManager->GetGpuVirtualAddress(mFrameData->GetSkeletonConstantBufferHandle(mCurrentFrame,pMeshHandle)));
			//mCommandList->SetGraphicsRootDescriptorTable(7, mResourceManager->GetGpuHandle(mDescriptorHandleMap[mMeshes[pMeshHandle]->GetSkeletonBufferHandle()]));
			mCommandList->SetGraphicsRoot32BitConstant(9, 1, 0);
		}
		else
		{
			mCommandList->SetGraphicsRoot32BitConstant(9, 0, 0);
		}

		mCommandList->SetGraphicsRoot32BitConstants(7, 10, &lPBRData, 0);

		mCommandList->IASetVertexBuffers(0, 1, &lVertexBufferView);

		for (int lSubMeshIndex = 0; lSubMeshIndex < mMeshes[pMeshHandle]->GetSubMeshCount(); ++lSubMeshIndex)
		{
			D3D12_INDEX_BUFFER_VIEW lIndexBufferView;
			lIndexBufferView.BufferLocation = mResourceManager->GetGpuVirtualAddress(mMeshes[pMeshHandle]->GetIndexBufferHandle(lSubMeshIndex));
			lIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
			lIndexBufferView.SizeInBytes = sizeof(uint16_t) * mMeshes[pMeshHandle]->GetIndexCount(lSubMeshIndex);

			mCommandList->IASetIndexBuffer(&lIndexBufferView);
			mCommandList->DrawIndexedInstanced(mMeshes[pMeshHandle]->GetIndexCount(lSubMeshIndex), 1, 0, 0, 0);
		}
	}



	void CRenderer::WaitUntilAllCommandDone()
	{
		++mFenceValue;

		mCommandQueue->Signal(mFence.Get(), mFenceValue);

		if (mFence->GetCompletedValue() < mFenceValue)
		{
			HANDLE event = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			mFence->SetEventOnCompletion(mFenceValue, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
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



	void CRenderer::CreateFence()
	{
		if (!SUCCEEDED(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()))))
			throw string("creating fence fails.");
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
		if (!SUCCEEDED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()))))
			throw string("creating command allocator fails.");

		D3D12_COMMAND_QUEUE_DESC lDesc = {};
		lDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		lDesc.NodeMask = 0;
		lDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		lDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

		if (!SUCCEEDED(mDevice->CreateCommandQueue(&lDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()))))
			throw string("creating command queue fails.");

		if (!SUCCEEDED(mDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()))))
			throw string("creating command list fails.");



		if (!SUCCEEDED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCopyCommandAllocator.GetAddressOf()))))
			throw string("creating copy command allocator fails.");

		D3D12_COMMAND_QUEUE_DESC lCopyDesc = {};
		lDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		lDesc.NodeMask = 0;
		lDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		lDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

		if (!SUCCEEDED(mDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCopyCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCopyCommandList.GetAddressOf()))))
			throw string("creating copy command list fails.");

		if (!SUCCEEDED(mCopyCommandList->Close()))
			throw string("closing copy command list fails.");
	}



	void CRenderer::CreateResourceManager()
	{
		mResourceManager = make_unique<CResourceManager>(mDevice.Get(), mCommandList.Get(), mCopyCommandList.Get());
	}



	void CRenderer::CreateFrameData()
	{
		mFrameData = make_unique<CFrameData>(mDevice.Get(), mFrameNum);
		for (int lFrameIndex = 0; lFrameIndex < mFrameNum; ++lFrameIndex)
		{
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
			mRtvHandle[lSwapChainIndex] = mResourceManager->CreateDescriptor(mSwapchainBufferHandle[lSwapChainIndex], EDescriptorType::eRTV);
		}
	}



	void CRenderer::CreateDepthBuffer()
	{
		mDepthBufferHandle = mResourceManager->CreateDepthTexture(mWidth, mHeight, 1, mDepthBufferFormat, EResourceHeapType::eDefault, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		mResourceManager->ChangeState(mDepthBufferHandle, D3D12_RESOURCE_STATE_DEPTH_WRITE);

		mDsvHandle = mResourceManager->CreateDescriptor(mDepthBufferHandle, EDescriptorType::eDSV);
	}



	void CRenderer::LoadShaders()
	{
		if (!SUCCEEDED(D3DReadFileToBlob(L"../Renderer/shader/DefaultVS.cso", mShaders["DefaultVS"].GetAddressOf())))
			throw string("read shader fails.");

		if (!SUCCEEDED(D3DReadFileToBlob(L"../Renderer/shader/DefaultPS.cso", mShaders["DefaultPS"].GetAddressOf())))
			throw string("read shader fails.");

		if (!SUCCEEDED(D3DReadFileToBlob(L"../Renderer/shader/LineVS.cso", mShaders["LineVS"].GetAddressOf())))
			throw string("read shader fails.");

		if (!SUCCEEDED(D3DReadFileToBlob(L"../Renderer/shader/LinePS.cso", mShaders["LinePS"].GetAddressOf())))
			throw string("read shader fails.");

		if (!SUCCEEDED(D3DReadFileToBlob(L"../Renderer/shader/PBRVS.cso", mShaders["PBRVS"].GetAddressOf())))
			throw string("read shader fails.");

		if (!SUCCEEDED(D3DReadFileToBlob(L"../Renderer/shader/PBRPS.cso", mShaders["PBRPS"].GetAddressOf())))
			throw string("read shader fails.");
	}



	void CRenderer::CreateRootSignatures()
	{
		/*
		* line root signature
		*/

		D3D12_ROOT_PARAMETER lLineParameter[2];
		lLineParameter[0].Constants.Num32BitValues = 9;
		lLineParameter[0].Constants.RegisterSpace = 0;
		lLineParameter[0].Constants.ShaderRegister = 0;
		lLineParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lLineParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		lLineParameter[1].Descriptor.RegisterSpace = 0;
		lLineParameter[1].Descriptor.ShaderRegister = 1;
		lLineParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lLineParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

		D3D12_ROOT_SIGNATURE_DESC lLineDesc = {};
		lLineDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
		lLineDesc.NumParameters = 2;
		lLineDesc.NumStaticSamplers = 0;
		lLineDesc.pParameters = lLineParameter;

		AddRootSignature("Line", lLineDesc);

		/*
		* default root signature
		*/

		D3D12_ROOT_PARAMETER lDefaultParameter[2];
		lDefaultParameter[0].Descriptor.RegisterSpace = 0;
		lDefaultParameter[0].Descriptor.ShaderRegister = 0;
		lDefaultParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lDefaultParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		lDefaultParameter[1].Descriptor.RegisterSpace = 0;
		lDefaultParameter[1].Descriptor.ShaderRegister = 1;
		lDefaultParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lDefaultParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

		D3D12_ROOT_SIGNATURE_DESC lDefaultDesc = {};
		lDefaultDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		lDefaultDesc.NumParameters = 2;
		lDefaultDesc.NumStaticSamplers = 0;
		lDefaultDesc.pParameters = lDefaultParameter;

		AddRootSignature("Default", lDefaultDesc);



		/*
		*  PBR root signature
		*/
		D3D12_DESCRIPTOR_RANGE lPBRDescriptorRange[5];
		lPBRDescriptorRange[0].BaseShaderRegister = 0;
		lPBRDescriptorRange[0].NumDescriptors = 1;
		lPBRDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		lPBRDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		lPBRDescriptorRange[0].RegisterSpace = 0;
		lPBRDescriptorRange[1].BaseShaderRegister = 1;
		lPBRDescriptorRange[1].NumDescriptors = 1;
		lPBRDescriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		lPBRDescriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		lPBRDescriptorRange[1].RegisterSpace = 0;
		lPBRDescriptorRange[2].BaseShaderRegister = 2;
		lPBRDescriptorRange[2].NumDescriptors = 1;
		lPBRDescriptorRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		lPBRDescriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		lPBRDescriptorRange[2].RegisterSpace = 0;
		lPBRDescriptorRange[3].BaseShaderRegister = 3;
		lPBRDescriptorRange[3].NumDescriptors = 1;
		lPBRDescriptorRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		lPBRDescriptorRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		lPBRDescriptorRange[3].RegisterSpace = 0;
		lPBRDescriptorRange[4].BaseShaderRegister = 4;
		lPBRDescriptorRange[4].NumDescriptors = 1;
		lPBRDescriptorRange[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		lPBRDescriptorRange[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		lPBRDescriptorRange[4].RegisterSpace = 0;

		D3D12_ROOT_PARAMETER lPBRParameter[10];
		lPBRParameter[0].Descriptor.RegisterSpace = 0;
		lPBRParameter[0].Descriptor.ShaderRegister = 0;
		lPBRParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		lPBRParameter[1].Descriptor.RegisterSpace = 0;
		lPBRParameter[1].Descriptor.ShaderRegister = 1;
		lPBRParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		lPBRParameter[2].DescriptorTable.NumDescriptorRanges = 1;
		lPBRParameter[2].DescriptorTable.pDescriptorRanges = &lPBRDescriptorRange[0];
		lPBRParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		lPBRParameter[3].DescriptorTable.NumDescriptorRanges = 1;
		lPBRParameter[3].DescriptorTable.pDescriptorRanges = &lPBRDescriptorRange[1];
		lPBRParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		lPBRParameter[4].DescriptorTable.NumDescriptorRanges = 1;
		lPBRParameter[4].DescriptorTable.pDescriptorRanges = &lPBRDescriptorRange[2];
		lPBRParameter[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		lPBRParameter[5].DescriptorTable.NumDescriptorRanges = 1;
		lPBRParameter[5].DescriptorTable.pDescriptorRanges = &lPBRDescriptorRange[3];
		lPBRParameter[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		lPBRParameter[6].DescriptorTable.NumDescriptorRanges = 1;
		lPBRParameter[6].DescriptorTable.pDescriptorRanges = &lPBRDescriptorRange[4];
		lPBRParameter[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		lPBRParameter[7].Constants.Num32BitValues = 10;
		lPBRParameter[7].Constants.RegisterSpace = 0;
		lPBRParameter[7].Constants.ShaderRegister = 2;
		lPBRParameter[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		lPBRParameter[8].Descriptor.RegisterSpace = 0;
		lPBRParameter[8].Descriptor.ShaderRegister = 3;
		lPBRParameter[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		lPBRParameter[9].Constants.Num32BitValues = 1;
		lPBRParameter[9].Constants.RegisterSpace = 0;
		lPBRParameter[9].Constants.ShaderRegister = 4;
		lPBRParameter[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		lPBRParameter[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;


		CD3DX12_STATIC_SAMPLER_DESC lPBRSamplerDesc(0,
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);

		D3D12_ROOT_SIGNATURE_DESC lPBRDesc = {};
		lPBRDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		lPBRDesc.NumParameters = 10;
		lPBRDesc.pParameters = lPBRParameter;
		lPBRDesc.NumStaticSamplers = 1;
		lPBRDesc.pStaticSamplers = &lPBRSamplerDesc;

		AddRootSignature("PBR", lPBRDesc);
	}



	void CRenderer::AddRootSignature(const char* pName,const D3D12_ROOT_SIGNATURE_DESC& pDesc)
	{
		ComPtr<ID3DBlob> lSerialized;
		ComPtr<ID3DBlob> lErrorMessage;
		ComPtr<ID3D12RootSignature> lRootSignature;

		if (!SUCCEEDED(D3D12SerializeRootSignature(&pDesc, D3D_ROOT_SIGNATURE_VERSION_1, lSerialized.GetAddressOf(), lErrorMessage.GetAddressOf())))
			throw string((char*)lErrorMessage->GetBufferPointer());

		if (!SUCCEEDED(mDevice->CreateRootSignature(0, lSerialized->GetBufferPointer(), lSerialized->GetBufferSize(), IID_PPV_ARGS(lRootSignature.GetAddressOf()))))
			throw string("creating root signature fails.");

		mRootSignatures[string(pName)] = move(lRootSignature);
	}



	void CRenderer::CreatePSO()
	{
		ComPtr<ID3D12PipelineState> lPSO;

		/*
		* line pipeline
		*/

		D3D12_GRAPHICS_PIPELINE_STATE_DESC lLinePsoDesc = {};

		lLinePsoDesc.InputLayout.NumElements = 0;
		lLinePsoDesc.pRootSignature = mRootSignatures["Line"].Get();
		lLinePsoDesc.VS.pShaderBytecode = mShaders["LineVS"]->GetBufferPointer();
		lLinePsoDesc.VS.BytecodeLength = mShaders["LineVS"]->GetBufferSize();
		lLinePsoDesc.PS.pShaderBytecode = mShaders["LinePS"]->GetBufferPointer();
		lLinePsoDesc.PS.BytecodeLength = mShaders["LinePS"]->GetBufferSize();
		lLinePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		lLinePsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		lLinePsoDesc.RasterizerState.FrontCounterClockwise = FALSE;
		lLinePsoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		lLinePsoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		lLinePsoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		lLinePsoDesc.RasterizerState.DepthClipEnable = TRUE;
		lLinePsoDesc.RasterizerState.MultisampleEnable = FALSE;
		lLinePsoDesc.RasterizerState.AntialiasedLineEnable = FALSE;
		lLinePsoDesc.RasterizerState.ForcedSampleCount = 0;
		lLinePsoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		lLinePsoDesc.BlendState.AlphaToCoverageEnable = FALSE;
		lLinePsoDesc.BlendState.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC lLineRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			lLinePsoDesc.BlendState.RenderTarget[i] = lLineRenderTargetBlendDesc;
		lLinePsoDesc.DepthStencilState.DepthEnable = TRUE;
		lLinePsoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		lLinePsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		lLinePsoDesc.DepthStencilState.StencilEnable = FALSE;
		lLinePsoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		lLinePsoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		const D3D12_DEPTH_STENCILOP_DESC lLineStencilOp =
		{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		lLinePsoDesc.DepthStencilState.FrontFace = lLineStencilOp;
		lLinePsoDesc.DepthStencilState.BackFace = lLineStencilOp;
		lLinePsoDesc.SampleMask = UINT_MAX;
		lLinePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		lLinePsoDesc.NumRenderTargets = 1;
		lLinePsoDesc.RTVFormats[0] = mBackBufferFormat;
		lLinePsoDesc.SampleDesc.Count = 1;
		lLinePsoDesc.SampleDesc.Quality = 0;
		lLinePsoDesc.DSVFormat = mDepthBufferFormat;

		if (!SUCCEEDED(mDevice->CreateGraphicsPipelineState(&lLinePsoDesc, IID_PPV_ARGS(lPSO.GetAddressOf()))))
			throw string("creating graphics pipeline state fails.");

		mPSOs["Line"] = move(lPSO);


		/*
		* default pipeline
		*/


		D3D12_GRAPHICS_PIPELINE_STATE_DESC lDefaultPsoDesc = {};

		D3D12_INPUT_ELEMENT_DESC lDefaultInputElements[1];
		lDefaultInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lDefaultPsoDesc.InputLayout.NumElements = 1;
		lDefaultPsoDesc.InputLayout.pInputElementDescs = lDefaultInputElements;
		lDefaultPsoDesc.pRootSignature = mRootSignatures["Default"].Get();
		lDefaultPsoDesc.VS.pShaderBytecode = mShaders["DefaultVS"]->GetBufferPointer();
		lDefaultPsoDesc.VS.BytecodeLength = mShaders["DefaultVS"]->GetBufferSize();
		lDefaultPsoDesc.PS.pShaderBytecode = mShaders["DefaultPS"]->GetBufferPointer();
		lDefaultPsoDesc.PS.BytecodeLength = mShaders["DefaultPS"]->GetBufferSize();
		lDefaultPsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		lDefaultPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		lDefaultPsoDesc.RasterizerState.FrontCounterClockwise = FALSE;
		lDefaultPsoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		lDefaultPsoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		lDefaultPsoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		lDefaultPsoDesc.RasterizerState.DepthClipEnable = TRUE;
		lDefaultPsoDesc.RasterizerState.MultisampleEnable = FALSE;
		lDefaultPsoDesc.RasterizerState.AntialiasedLineEnable = FALSE;
		lDefaultPsoDesc.RasterizerState.ForcedSampleCount = 0;
		lDefaultPsoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		lDefaultPsoDesc.BlendState.AlphaToCoverageEnable = FALSE;
		lDefaultPsoDesc.BlendState.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC lDefaultRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			lDefaultPsoDesc.BlendState.RenderTarget[i] = lDefaultRenderTargetBlendDesc;
		lDefaultPsoDesc.DepthStencilState.DepthEnable = TRUE;
		lDefaultPsoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		lDefaultPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		lDefaultPsoDesc.DepthStencilState.StencilEnable = FALSE;
		lDefaultPsoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		lDefaultPsoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		const D3D12_DEPTH_STENCILOP_DESC lDefaultStencilOp =
		{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		lDefaultPsoDesc.DepthStencilState.FrontFace = lDefaultStencilOp;
		lDefaultPsoDesc.DepthStencilState.BackFace = lDefaultStencilOp;
		lDefaultPsoDesc.SampleMask = UINT_MAX;
		lDefaultPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		lDefaultPsoDesc.NumRenderTargets = 1;
		lDefaultPsoDesc.RTVFormats[0] = mBackBufferFormat;
		lDefaultPsoDesc.SampleDesc.Count = 1;
		lDefaultPsoDesc.SampleDesc.Quality = 0;
		lDefaultPsoDesc.DSVFormat = mDepthBufferFormat;

		if (!SUCCEEDED(mDevice->CreateGraphicsPipelineState(&lDefaultPsoDesc, IID_PPV_ARGS(lPSO.GetAddressOf()))))
			throw string("creating graphics pipeline state fails.");

		mPSOs["Default"] = move(lPSO);


		/*
		* PBR pipeline
		*/
		D3D12_GRAPHICS_PIPELINE_STATE_DESC lPBRPsoDesc = {};

		D3D12_INPUT_ELEMENT_DESC lPBRInputElements[7];
		lPBRInputElements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRInputElements[1] = { "TEX",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRInputElements[2] = { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRInputElements[3] = { "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,32,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRInputElements[4] = { "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,44,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRInputElements[5] = { "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,56,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRInputElements[6] = { "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32_FLOAT,0,72,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA ,0 };
		lPBRPsoDesc.InputLayout.NumElements = 7;
		lPBRPsoDesc.InputLayout.pInputElementDescs = lPBRInputElements;
		lPBRPsoDesc.pRootSignature = mRootSignatures["PBR"].Get();
		lPBRPsoDesc.VS.pShaderBytecode = mShaders["PBRVS"]->GetBufferPointer();
		lPBRPsoDesc.VS.BytecodeLength = mShaders["PBRVS"]->GetBufferSize();
		lPBRPsoDesc.PS.pShaderBytecode = mShaders["PBRPS"]->GetBufferPointer();
		lPBRPsoDesc.PS.BytecodeLength = mShaders["PBRPS"]->GetBufferSize();
		lPBRPsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		lPBRPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		lPBRPsoDesc.RasterizerState.FrontCounterClockwise = FALSE;
		lPBRPsoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		lPBRPsoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		lPBRPsoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		lPBRPsoDesc.RasterizerState.DepthClipEnable = TRUE;
		lPBRPsoDesc.RasterizerState.MultisampleEnable = FALSE;
		lPBRPsoDesc.RasterizerState.AntialiasedLineEnable = FALSE;
		lPBRPsoDesc.RasterizerState.ForcedSampleCount = 0;
		lPBRPsoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		lPBRPsoDesc.BlendState.AlphaToCoverageEnable = FALSE;
		lPBRPsoDesc.BlendState.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC lPBRRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			lPBRPsoDesc.BlendState.RenderTarget[i] = lPBRRenderTargetBlendDesc;
		lPBRPsoDesc.DepthStencilState.DepthEnable = TRUE;
		lPBRPsoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		lPBRPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		lPBRPsoDesc.DepthStencilState.StencilEnable = FALSE;
		lPBRPsoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		lPBRPsoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		const D3D12_DEPTH_STENCILOP_DESC lPBRStencilOp =
		{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		lPBRPsoDesc.DepthStencilState.FrontFace = lPBRStencilOp;
		lPBRPsoDesc.DepthStencilState.BackFace = lPBRStencilOp;
		lPBRPsoDesc.SampleMask = UINT_MAX;
		lPBRPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		lPBRPsoDesc.NumRenderTargets = 1;
		lPBRPsoDesc.RTVFormats[0] = mBackBufferFormat;
		lPBRPsoDesc.SampleDesc.Count = 1;
		lPBRPsoDesc.SampleDesc.Quality = 0;
		lPBRPsoDesc.DSVFormat = mDepthBufferFormat;

		if (!SUCCEEDED(mDevice->CreateGraphicsPipelineState(&lPBRPsoDesc, IID_PPV_ARGS(lPSO.GetAddressOf()))))
			throw string("creating graphics pipeline state fails.");

		mPSOs["PBR"] = move(lPSO);
	}



	void CRenderer::SetViewportAndScissor()
	{
		mViewport.TopLeftX = 0;
		mViewport.TopLeftY = 0;
		mViewport.Width = static_cast<float>(mWidth);
		mViewport.Height = static_cast<float>(mHeight);
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		mScissor.left = 0;
		mScissor.top = 0;
		mScissor.right = mWidth;
		mScissor.bottom = mHeight;
	}



	void CRenderer::UploadWorldConstantBuffer()
	{
		SWorldData lWorldData;
		lWorldData.mViewMatrix = mCamera->GetViewMatrix();
		lWorldData.mProjectionMatrix = mCamera->GetProjectionMatrix();
		mResourceManager->Upload(mFrameData->GetWorldConstantBufferHandle(mCurrentFrame), &lWorldData, sizeof(SWorldData), 1, 0, 0);
	}

	void CRenderer::UploadObjectConstantBuffer()
	{
		for (int lObjectIndex = 0; lObjectIndex < mObjectCount; ++lObjectIndex)
		{
			SObjectData lObjectData;

			lObjectData.mWorldMatrix = mObjects[lObjectIndex]->GetWorldMatrix();

			mResourceManager->Upload(mFrameData->GetObjectConstantBufferHandle(mCurrentFrame, lObjectIndex), &lObjectData, sizeof(SObjectData), 1, 0, 0);
		}
	}

	void CRenderer::UploadSkeletonConstantBuffer()
	{
		for (int lMeshIndex = 0; lMeshIndex < mMeshCount; ++lMeshIndex)
		{
			if (!mMeshes[lMeshIndex]->HasSkeleton())
				continue;

			vector<Math::SMatrix4> lBoneTransformMatrices;

			mMeshes[lMeshIndex]->GetBoneTransformMatrices(lBoneTransformMatrices);

			mResourceManager->Upload(mFrameData->GetSkeletonConstantBufferHandle(mCurrentFrame,lMeshIndex), lBoneTransformMatrices.data(), sizeof(Math::SMatrix4) * mMeshes[lMeshIndex]->GetBoneCount(), 1, 0, 0);
		}
	}
}
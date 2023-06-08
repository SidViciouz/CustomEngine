#pragma once

#include "Common.h"

namespace Renderer
{
	class CResourceManager;
	class CFrameData;
	class CCamera;
	class CMesh;
	class CObject;

	class CRenderer
	{
	public:
												CRenderer(HWND pWindowHandle);
												~CRenderer();
		void									Initialize();
		void									Resize();
		void									SetCamera(shared_ptr<CCamera> pCamera);
		int										SetMesh(shared_ptr<CMesh> pMesh);
		int										SetObject(shared_ptr<CObject> pObject);

		void									LoadBegin();
		void									LoadEnd();
		int										LoadTexture(const wchar_t* pFilePath);

		void									DrawBegin();
		void									DrawEnd();
		void									DrawLine(void* pData);
		void									DrawAxis();
		void									DrawSquare();
		void									DrawCube();
		void									DrawMesh(int pMeshHandle, int pObjectHandle);
		void									DrawMeshPBR(int pMeshHandle, int pObjectHandle,int pBaseColorTextureHandle, int pMetallicTextureHandle, int pNormalTextureHandle, int pRoughnessTextureHandle, int pAmbientOcculstionTextureHandle);

	private:
		/*
		* window data
		*/
		HWND									mWindowHandle;
		int										mWidth;
		int										mHeight;

		/*
		* activate debug mode
		*/
		void									EnableDebug();
		bool									mDebugEnabled = false;
		ComPtr<ID3D12Debug>						mDebugController;
		/*
		* create factory and device
		*/
		void									CreateDevice();
		ComPtr<IDXGIFactory4>					mFactory;
		ComPtr<ID3D12Device>					mDevice;

		/*
		* create Fence
		*/
		void									CreateFence();
		ComPtr<ID3D12Fence>						mFence;
		UINT64									mFenceValue = 0;
		

		/*
		* adaptor infos.
		*/
		void									InitAdaptInfo();
		IDXGIAdapter*							mAdapters[MAX_ADAPTER_NUM];
		wchar_t									mAdapterNames[MAX_ADAPTER_NUM][128];
		int										mAdapterNum = 0;
		int										mCurrentAdapter = 0;

		/*
		* create command objects
		*/
		void									CreateCommandObjects();
		ComPtr<ID3D12CommandQueue>				mCommandQueue;
		ComPtr<ID3D12CommandAllocator>			mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList>		mCommandList;
		ComPtr<ID3D12CommandAllocator>			mCopyCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList>		mCopyCommandList;

		/*
		* resource management
		*/
		void									CreateResourceManager();
		unique_ptr<CResourceManager>			mResourceManager;

		/*
		* frame data (some constant buffer, command allocator and fence)
		*/
		void									CreateFrameData();
		int										mCurrentFrame = 0;
		int										mFrameNum = FRAMEDATA_NUM;
		unique_ptr<CFrameData>					mFrameData;

		/*
		* swap chain
		*/
		void									CreateSwapchain();
		ComPtr<IDXGISwapChain>					mSwapchain;
		int										mSwapchainBufferNum = SWAPCHAIN_BUFFER_NUM;
		int										mCurrentBackBuffer = 0;
		int										mSwapchainBufferHandle[SWAPCHAIN_BUFFER_NUM];
		int										mRtvHandle[SWAPCHAIN_BUFFER_NUM];
		DXGI_FORMAT								mBackBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;

		/*
		* Depth stencil buffer
		*/
		void									CreateDepthBuffer();
		int										mDepthBufferHandle;
		int										mDsvHandle;
		DXGI_FORMAT								mDepthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		/*
		* load shader programs
		*/
		void									LoadShaders();
		unordered_map<string,ComPtr<ID3DBlob>>	mShaders;

		/*
		* create root signature
		* i will make root signature builder later
		*/
		void									CreateRootSignatures();
		void									AddRootSignature(const char* pName,const D3D12_ROOT_SIGNATURE_DESC& pDesc);
		unordered_map<string, ComPtr<ID3D12RootSignature>> mRootSignatures;

		/*
		* create pso
		*/
		void									CreatePSO();
		unordered_map<string, ComPtr<ID3D12PipelineState>> mPSOs;

		/*
		* viewport and scissor
		*/
		void									SetViewportAndScissor();
		D3D12_VIEWPORT							mViewport;
		D3D12_RECT								mScissor;

		/*
		* camera data
		*/
		void									UploadWorldConstantBuffer();
		shared_ptr<CCamera>						mCamera;

		/*
		* mesh data
		*/
		void									UploadSkeletonConstantBuffer();
		vector<shared_ptr<CMesh>>				mMeshes;
		int										mMeshCount = 0;

		void									UploadObjectConstantBuffer();
		int										mObjectCount = 0;
		vector<shared_ptr<CObject>>				mObjects;

		
		/*
		* texture data
		*/
		unordered_map<int, int>					mUploadBufferHandleMap; // textureHandle -> uploadBufferHandle
		unordered_map<int, int>					mDescriptorHandleMap; // textureHandle -> DescriptorHandle
	};
}
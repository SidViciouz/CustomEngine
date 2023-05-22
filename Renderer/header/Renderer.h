#pragma once

#include "Common.h"

namespace Renderer
{
	class CResourceManager;

	class CRenderer
	{
	public:
												CRenderer();
												~CRenderer();
		void									Initialize();
		void									Run();

	private:
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
		ComPtr<ID3D12CommandList>				mCommandList;

		/*
		* resource management
		*/
		void									CreateResourceManager();
		unique_ptr<CResourceManager>			mResourceManager;
	};
}
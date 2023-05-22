#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_4.h>

#define MAX_ADAPTER_NUM 5

using namespace Microsoft::WRL;

namespace Renderer
{
	class CRenderer
	{
	public:
												CRenderer();
												~CRenderer();
		void									Initialize();

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

		/*
		* create command objects
		*/
		void									CreateCommandObjects();
		ComPtr<ID3D12CommandQueue>				mCommandQueue;
		ComPtr<ID3D12CommandAllocator>			mCommandAllocator;
		ComPtr<ID3D12CommandList>				mCommandList;
	};
}
#include "Renderer.h"

#include <iostream>
#include <vector>

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
			throw std::string("enabling debug fails.");
		}

	}

	void CRenderer::CreateDevice()
	{
		if (!SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(mFactory.GetAddressOf()))))
			throw std::string("create dxgi factory1 fails.");
		
		InitAdaptInfo();

		if (!SUCCEEDED(D3D12CreateDevice(mAdapters[0], D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&mDevice))))
			throw std::string("create device fails.");
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
			std::wcout << mAdapterNames[i];
	}

	void CRenderer::CreateCommandObjects()
	{
		mDevice->CreateCommandAllocator(D3D12)
	}
}
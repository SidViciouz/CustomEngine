#include "FrameData.h"

namespace Renderer
{
	CFrameData::CFrameData(ID3D12Device* pDevice,int pNum)
		: mDevice{ pDevice }, mFrameNum{ pNum }, mFrameStats(pNum)
	{
		for (int lFrameIndex = 0; lFrameIndex < mFrameNum; ++lFrameIndex)
		{
			if (!SUCCEEDED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mFrameStats[lFrameIndex].mCommandAllocator.GetAddressOf()))))
				throw string("creating command allocator fails.");
		}
	}

	CFrameData::~CFrameData()
	{

	}

	void CFrameData::SetObjectConstantBufferHandle(int pIndex, int pHandle)
	{
		mFrameStats[pIndex].mObjectConstantBufferHandle = pHandle;
	}

	void CFrameData::SetWorldConstantBufferHandle(int pIndex, int pHandle)
	{
		mFrameStats[pIndex].mWorldConstantBufferHandle = pHandle;
	}
}
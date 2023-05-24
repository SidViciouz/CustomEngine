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

	int CFrameData::GetObjectConstantBufferHandle(int pIndex) const
	{
		return mFrameStats[pIndex].mObjectConstantBufferHandle;
	}

	void CFrameData::SetWorldConstantBufferHandle(int pIndex, int pHandle)
	{
		mFrameStats[pIndex].mWorldConstantBufferHandle = pHandle;
	}

	int CFrameData::GetWorldConstantBufferHandle(int pIndex) const
	{
		return mFrameStats[pIndex].mWorldConstantBufferHandle;
	}

	UINT64 CFrameData::GetFenceValue(int pIndex) const
	{
		return mFrameStats[pIndex].mFenceValue;
	}

	void CFrameData::SetFenceValue(int pIndex,UINT64 pFenceValue)
	{
		mFrameStats[pIndex].mFenceValue = pFenceValue;
	}

	ID3D12CommandAllocator* CFrameData::GetCommandAllocator(int pIndex) const
	{
		return mFrameStats[pIndex].mCommandAllocator.Get();
	}
}
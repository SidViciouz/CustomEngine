#pragma once

#include "Common.h"

namespace Renderer
{
	struct SFrameStats
	{
		UINT64									mFenceValue = 0;
		int										mObjectConstantBufferHandle[MAX_OBJECT_NUM] = {-1,};
		int										mWorldConstantBufferHandle = -1;
		int										mSkeletonConstantBufferHandle[MAX_OBJECT_NUM] = { -1, };
		ComPtr<ID3D12CommandAllocator>			mCommandAllocator;
	};

	class CFrameData
	{
	public:
												CFrameData(ID3D12Device* pDevice,int pNum);
												~CFrameData();

		void									SetObjectConstantBufferHandle(int pFrameIndex,int pObjectHandle,int pHandle);
		int										GetObjectConstantBufferHandle(int pFrameIndex, int pObjectHandle) const;

		void									SetWorldConstantBufferHandle(int pIndex, int pHandle);
		int 									GetWorldConstantBufferHandle(int pIndex) const;

		void									SetSkeletonConstantBufferHandle(int pFrameIndex, int pObjectHandle, int pHandle);
		int										GetSkeletonConstantBufferHandle(int pFrameIndex, int pObjectHandle) const;

		UINT64									GetFenceValue(int pIndex) const;
		void									SetFenceValue(int pIndex,UINT64 pFenceValue);

		ID3D12CommandAllocator*					GetCommandAllocator(int pIndex) const;

	private:

		vector<SFrameStats>						mFrameStats;

		const int								mFrameNum;

		ID3D12Device*							mDevice;
	};
}

#pragma once

#include "Common.h"

namespace Renderer
{
	struct SFrameStats
	{
		UINT64									mFenceValue = 0;
		int										mObjectConstantBufferHandle = -1;
		int										mWorldConstantBufferHandle = -1;
		ComPtr<ID3D12CommandAllocator>			mCommandAllocator;
	};

	class CFrameData
	{
	public:
												CFrameData(ID3D12Device* pDevice,int pNum);
												~CFrameData();

		void									SetObjectConstantBufferHandle(int pIndex,int pHandle);
		void									SetWorldConstantBufferHandle(int pIndex,int pHandle);

	private:

		vector<SFrameStats>						mFrameStats;

		const int								mFrameNum;

		ID3D12Device*							mDevice;
	};
}

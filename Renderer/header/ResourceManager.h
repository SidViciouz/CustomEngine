#pragma once

#include "Common.h"

namespace Renderer
{
	enum class EResourceHeapType
	{
		eDefault,
		eUpload,
		eReadBack,
		eCustom
	};

	enum class EResourceDimension
	{
		eBuffer,
		eTexture1D,
		eTexture2D,
		eTexture3D,
		eUnknown
	};

	struct SResourceDescription
	{
		EResourceHeapType					mHeapType;
		EResourceDimension					mDimension;
		int									mWidth;
		int									mHeight;
		int									mDepth;
		DXGI_FORMAT							mFormat;
		D3D12_TEXTURE_LAYOUT				mLayout;
		D3D12_RESOURCE_FLAGS				mFlags;
		int									mAlignment;
		int									mMipLevel;
	};

	struct SResourceInfo
	{
		D3D12_RESOURCE_STATES				mState;
		ComPtr<ID3D12Resource>				mResource;
	};

	class CResourceManager
	{
	public:
											CResourceManager(ID3D12Device* pDevice);
											~CResourceManager();

		/*
		* create buffer and return resource handle.
		*/
		int									CreateBuffer(int pByteSize, EResourceHeapType pType);
		int									CreateTexture1D(int pWidth, DXGI_FORMAT pFormat, EResourceHeapType pType,D3D12_RESOURCE_FLAGS pFlags);
		int									CreateTexture2D(int pWidth, int pHeight, int pArraySize, DXGI_FORMAT pFormat, EResourceHeapType pType, D3D12_RESOURCE_FLAGS pFlags);
		int									CreateTexture3D(int pWidth, int pHeight, int pDepth, DXGI_FORMAT pFormat, EResourceHeapType pType, D3D12_RESOURCE_FLAGS pFlags);

	private:
		/*
		* create buffer and return decriptor handle.
		*/
		int									CreateResource(const SResourceDescription& pDescrption);

		ID3D12Device*						mDevice;

		array<SResourceInfo, MAX_RESOURCE_NUM> mResources;
		int									mResourceCount = 0;

		int									mDescriptorCount = 0;
	};
}
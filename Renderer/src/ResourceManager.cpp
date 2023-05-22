#include "ResourceManager.h"

namespace Renderer
{
	
	CResourceManager::CResourceManager(ID3D12Device* pDevice)
		:mDevice{ pDevice }
	{

	}

	CResourceManager::~CResourceManager()
	{

	}
	
	int CResourceManager::CreateBuffer(int pByteSize, EResourceHeapType pType)
	{
		SResourceDescription lResourceDescription = {};
		lResourceDescription.mDimension = EResourceDimension::eBuffer;
		lResourceDescription.mAlignment = 0;
		lResourceDescription.mWidth = pByteSize;
		lResourceDescription.mHeight = 1;
		lResourceDescription.mDepth = 1;
		lResourceDescription.mFormat = DXGI_FORMAT_UNKNOWN;
		lResourceDescription.mHeapType = pType;
		lResourceDescription.mLayout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		lResourceDescription.mMipLevel = 1;
		lResourceDescription.mFlags = D3D12_RESOURCE_FLAG_NONE;

		return CreateResource(lResourceDescription);
	}

	int CResourceManager::CreateTexture1D(int pWidth, DXGI_FORMAT pFormat, EResourceHeapType pType, D3D12_RESOURCE_FLAGS pFlags)
	{
		SResourceDescription lResourceDescription = {};
		lResourceDescription.mDimension = EResourceDimension::eTexture1D;
		lResourceDescription.mAlignment = 0;
		lResourceDescription.mWidth = pWidth;
		lResourceDescription.mHeight = 1;
		lResourceDescription.mDepth = 1;
		lResourceDescription.mFormat = pFormat;
		lResourceDescription.mHeapType = pType;
		lResourceDescription.mLayout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		lResourceDescription.mMipLevel = 1;
		lResourceDescription.mFlags = pFlags;

		return CreateResource(lResourceDescription);
	}

	int CResourceManager::CreateTexture2D(int pWidth, int pHeight, int pArraySize, DXGI_FORMAT pFormat, EResourceHeapType pType, D3D12_RESOURCE_FLAGS pFlags)
	{
		SResourceDescription lResourceDescription = {};
		lResourceDescription.mDimension = EResourceDimension::eTexture2D;
		lResourceDescription.mAlignment = 0;
		lResourceDescription.mWidth = pWidth;
		lResourceDescription.mHeight = pHeight;
		lResourceDescription.mDepth = pArraySize;
		lResourceDescription.mFormat = pFormat;
		lResourceDescription.mHeapType = pType;
		lResourceDescription.mLayout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		lResourceDescription.mMipLevel = 1;
		lResourceDescription.mFlags = pFlags;

		return CreateResource(lResourceDescription);
	}

	int CResourceManager::CreateTexture3D(int pWidth, int pHeight, int pDepth, DXGI_FORMAT pFormat, EResourceHeapType pType, D3D12_RESOURCE_FLAGS pFlags)
	{
		SResourceDescription lResourceDescription = {};
		lResourceDescription.mDimension = EResourceDimension::eTexture3D;
		lResourceDescription.mAlignment = 0;
		lResourceDescription.mWidth = pWidth;
		lResourceDescription.mHeight = pHeight;
		lResourceDescription.mDepth = pDepth;
		lResourceDescription.mFormat = pFormat;
		lResourceDescription.mHeapType = pType;
		lResourceDescription.mLayout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		lResourceDescription.mMipLevel = 1;
		lResourceDescription.mFlags = pFlags;
		
		return CreateResource(lResourceDescription);
	}

	int	CResourceManager::CreateResource(const SResourceDescription& pDescrption)
	{
		//heap description
		D3D12_HEAP_PROPERTIES lHeapProperties = {};
		lHeapProperties.VisibleNodeMask = 0;
		lHeapProperties.CreationNodeMask = 0;
		lHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		lHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		switch (pDescrption.mHeapType)
		{
		case EResourceHeapType::eDefault :
			lHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; break;

		case EResourceHeapType::eUpload:
			lHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; break;

		case EResourceHeapType::eReadBack:
			lHeapProperties.Type = D3D12_HEAP_TYPE_READBACK; break;

		// this case is not supported yet.
		case EResourceHeapType::eCustom:
			//lHeapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; break;
			throw std::string("creating buffer with custom type is not supported yet."); break;
		}


		//resrouce description
		D3D12_RESOURCE_DESC lResourceDesc = {};

		switch (pDescrption.mDimension)
		{
		case EResourceDimension::eBuffer :
			lResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; break;

		case EResourceDimension::eTexture1D:
			lResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D; break;

		case EResourceDimension::eTexture2D:
			lResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; break;

		case EResourceDimension::eTexture3D:
			lResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D; break;

		case EResourceDimension::eUnknown:
			lResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN; break;
		}
		
		lResourceDesc.Alignment = pDescrption.mAlignment;
		lResourceDesc.Width = pDescrption.mWidth;
		lResourceDesc.Height = pDescrption.mHeight;
		lResourceDesc.DepthOrArraySize = pDescrption.mHeight;
		lResourceDesc.MipLevels = pDescrption.mMipLevel;
		lResourceDesc.Format = pDescrption.mFormat;
		lResourceDesc.SampleDesc.Count = 1;
		lResourceDesc.SampleDesc.Quality = 0;
		lResourceDesc.Layout = pDescrption.mLayout;
		lResourceDesc.Flags = pDescrption.mFlags;


		SResourceInfo& lResourceInfo = mResources[mResourceCount];

		if (pDescrption.mHeapType == EResourceHeapType::eUpload)
			lResourceInfo.mState = D3D12_RESOURCE_STATE_GENERIC_READ;

		else if (pDescrption.mHeapType == EResourceHeapType::eReadBack)
			lResourceInfo.mState = D3D12_RESOURCE_STATE_COPY_DEST;

		else
			lResourceInfo.mState = D3D12_RESOURCE_STATE_GENERIC_READ;

		mDevice->CreateCommittedResource(&lHeapProperties, D3D12_HEAP_FLAG_NONE, &lResourceDesc,
			lResourceInfo.mState, nullptr, IID_PPV_ARGS(lResourceInfo.mResource.GetAddressOf()));


		return mResourceCount++;
	}

}
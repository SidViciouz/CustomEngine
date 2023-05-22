#include "ResourceManager.h"

namespace Renderer
{
	
	CResourceManager::CResourceManager(ID3D12Device* pDevice)
		:mDevice{ pDevice }
	{
		D3D12_DESCRIPTOR_HEAP_DESC lHeapDesc = {};

		lHeapDesc.NodeMask = 0;
		lHeapDesc.NumDescriptors = MAX_RTV_NUM;
		lHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		lHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		if (!SUCCEEDED(mDevice->CreateDescriptorHeap(&lHeapDesc, IID_PPV_ARGS(mDescriptorHeaps[0].GetAddressOf()))))
			throw string("creating descriptor heap fails.");

		lHeapDesc.NodeMask = 0;
		lHeapDesc.NumDescriptors = MAX_DSV_NUM;
		lHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		lHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		if (!SUCCEEDED(mDevice->CreateDescriptorHeap(&lHeapDesc, IID_PPV_ARGS(mDescriptorHeaps[1].GetAddressOf()))))
			throw string("creating descriptor heap fails.");

		lHeapDesc.NodeMask = 0;
		lHeapDesc.NumDescriptors = MAX_DESCRIPTOR_NUM;
		lHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		lHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		if (!SUCCEEDED(mDevice->CreateDescriptorHeap(&lHeapDesc, IID_PPV_ARGS(mDescriptorHeaps[2].GetAddressOf()))))
			throw string("creating descriptor heap fails.");


		mRtvIncSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvIncSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvUavIncSize =  mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		mRtvCpuHandleStart = mDescriptorHeaps[0]->GetCPUDescriptorHandleForHeapStart();
		mRtvGpuHandleStart = mDescriptorHeaps[0]->GetGPUDescriptorHandleForHeapStart();
		mDsvCpuHandleStart = mDescriptorHeaps[1]->GetCPUDescriptorHandleForHeapStart();
		mDsvGpuHandleStart = mDescriptorHeaps[1]->GetGPUDescriptorHandleForHeapStart();
		mCbvSrvUavCpuHandleStart = mDescriptorHeaps[2]->GetCPUDescriptorHandleForHeapStart();
		mCbvSrvUavGpuHandleStart = mDescriptorHeaps[2]->GetGPUDescriptorHandleForHeapStart();
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

		if (!SUCCEEDED(mDevice->CreateCommittedResource(&lHeapProperties, D3D12_HEAP_FLAG_NONE, &lResourceDesc,
			lResourceInfo.mState, nullptr, IID_PPV_ARGS(lResourceInfo.mResource.GetAddressOf()))))
			return -1;

		return mResourceCount++;
	}



	int	CResourceManager::CreateDescriptor(int pResourceHandle, EDescriptorType pType)
	{
		D3D12_RESOURCE_DESC lResourceDesc = mResources[pResourceHandle].mResource->GetDesc();

		if (pType == EDescriptorType::eRTV)
		{
			mDescriptorHandleToHeap[mDescriptorCount] = { 0, mRtvCount};
			mDevice->CreateRenderTargetView(mResources[pResourceHandle].mResource.Get(), nullptr, GetCpuHandle(EDescriptorType::eRTV,mRtvCount));

			++mRtvCount;

			return mDescriptorCount++;
		}

		else if(pType == EDescriptorType::eDSV)
		{
			mDescriptorHandleToHeap[mDescriptorCount] = { 1, mDsvCount};
			mDevice->CreateDepthStencilView(mResources[pResourceHandle].mResource.Get(), nullptr, GetCpuHandle(EDescriptorType::eDSV, mDsvCount));

			++mDsvCount;

			return mDescriptorCount++;
		}

		else // CBV, SRV, UAV
		{
			mDescriptorHandleToHeap[mDescriptorCount] = { 2, mCbvSrvUavCount};
			if (pType == EDescriptorType::eCBV)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC lCbvDesc = { };
				lCbvDesc.BufferLocation = mResources[pResourceHandle].mResource->GetGPUVirtualAddress();
				lCbvDesc.SizeInBytes = lResourceDesc.Width;
				mDevice->CreateConstantBufferView(&lCbvDesc, GetCpuHandle(EDescriptorType::eCBV, mCbvSrvUavCount));
			}

			else if (pType == EDescriptorType::eSRV)
			{
				D3D12_SRV_DIMENSION lDimension;
				switch(lResourceDesc.Dimension)
				{
				case D3D12_RESOURCE_DIMENSION_BUFFER:
					throw string("buffer dimension resource must be dealt with in other method."); break;

				case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
					lDimension = D3D12_SRV_DIMENSION_TEXTURE1D; break;

				case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
					lDimension = D3D12_SRV_DIMENSION_TEXTURE2D; break;

				case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
					lDimension = D3D12_SRV_DIMENSION_TEXTURE3D; break;

				case D3D12_RESOURCE_DIMENSION_UNKNOWN:
					lDimension = D3D12_SRV_DIMENSION_UNKNOWN; break;
				}

				D3D12_SHADER_RESOURCE_VIEW_DESC lSrvDesc = {};
				lSrvDesc.Format = lResourceDesc.Format;
				lSrvDesc.ViewDimension = lDimension;
				lSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

				if (lDimension == D3D12_SRV_DIMENSION_TEXTURE1D)
				{
					lSrvDesc.Texture1D.MipLevels = -1;
					lSrvDesc.Texture1D.MostDetailedMip = 0;
					lSrvDesc.Texture1D.ResourceMinLODClamp = 0.0f;
				}
				else if (lDimension == D3D12_SRV_DIMENSION_TEXTURE2D)
				{
					lSrvDesc.Texture2D.MipLevels = -1;
					lSrvDesc.Texture2D.MostDetailedMip = 0;
					lSrvDesc.Texture2D.PlaneSlice = 0;
					lSrvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
				}
				else if (lDimension == D3D12_SRV_DIMENSION_TEXTURE3D)
				{
					lSrvDesc.Texture3D.MipLevels = -1;
					lSrvDesc.Texture3D.MostDetailedMip = 0;
					lSrvDesc.Texture3D.ResourceMinLODClamp = 0.0f;
				}
				else
				{
					throw string("creating SRV fails.");
				}

				mDevice->CreateShaderResourceView(mResources[pResourceHandle].mResource.Get(), &lSrvDesc, GetCpuHandle(EDescriptorType::eSRV, mCbvSrvUavCount));
			}

			else //UAV
			{
				D3D12_UAV_DIMENSION lDimension;
				switch (lResourceDesc.Dimension)
				{
				case D3D12_RESOURCE_DIMENSION_BUFFER:
					throw string("buffer dimension resource must be dealt with in other method."); break;

				case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
					lDimension = D3D12_UAV_DIMENSION_TEXTURE1D; break;

				case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
					lDimension = D3D12_UAV_DIMENSION_TEXTURE2D; break;

				case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
					lDimension = D3D12_UAV_DIMENSION_TEXTURE3D; break;

				case D3D12_RESOURCE_DIMENSION_UNKNOWN:
					lDimension = D3D12_UAV_DIMENSION_UNKNOWN; break;
				}

				D3D12_UNORDERED_ACCESS_VIEW_DESC lUavDesc = {};
				lUavDesc.Format = lResourceDesc.Format;
				lUavDesc.ViewDimension = lDimension;
				
				if (lDimension == D3D12_UAV_DIMENSION_TEXTURE1D)
				{
					lUavDesc.Texture1D.MipSlice = 0;
				}
				else if (lDimension == D3D12_UAV_DIMENSION_TEXTURE2D)
				{
					lUavDesc.Texture2D.MipSlice = 0;
					lUavDesc.Texture2D.PlaneSlice = 0;

				}
				else if (lDimension == D3D12_UAV_DIMENSION_TEXTURE3D)
				{
					lUavDesc.Texture3D.FirstWSlice = 0;
					lUavDesc.Texture3D.MipSlice = 0;
					lUavDesc.Texture3D.WSize = -1;
				}
				else
				{
					throw string("creating UAV fails.");
				}

				mDevice->CreateUnorderedAccessView(mResources[pResourceHandle].mResource.Get(), nullptr, &lUavDesc, GetCpuHandle(EDescriptorType::eUAV, mCbvSrvUavCount));
			}

			++mCbvSrvUavCount;

			return mDescriptorCount++;
		}
	}



	D3D12_CPU_DESCRIPTOR_HANDLE CResourceManager::GetCpuHandle(EDescriptorType pType, int pHandle)
	{
		if (pType == EDescriptorType::eRTV)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE lCpuHandle = mRtvCpuHandleStart;
			lCpuHandle.ptr += pHandle * mRtvIncSize;

			return lCpuHandle;
		}
		else if (pType == EDescriptorType::eDSV)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE lCpuHandle = mDsvCpuHandleStart;
			lCpuHandle.ptr += pHandle * mDsvIncSize;

			return lCpuHandle;
		}
		else
		{
			D3D12_CPU_DESCRIPTOR_HANDLE lCpuHandle = mCbvSrvUavCpuHandleStart;
			lCpuHandle.ptr += pHandle * mCbvSrvUavIncSize;

			return lCpuHandle;
		}
	}



	D3D12_GPU_DESCRIPTOR_HANDLE	CResourceManager::GetGpuHandle(EDescriptorType pType, int pHandle)
	{
		if (pType == EDescriptorType::eRTV)
		{
			D3D12_GPU_DESCRIPTOR_HANDLE lGpuHandle = mRtvGpuHandleStart;
			lGpuHandle.ptr += pHandle * mRtvIncSize;

			return lGpuHandle;
		}
		else if (pType == EDescriptorType::eDSV)
		{
			D3D12_GPU_DESCRIPTOR_HANDLE lGpuHandle = mDsvGpuHandleStart;
			lGpuHandle.ptr += pHandle * mDsvIncSize;

			return lGpuHandle;
		}
		else
		{
			D3D12_GPU_DESCRIPTOR_HANDLE lGpuHandle = mCbvSrvUavGpuHandleStart;
			lGpuHandle.ptr += pHandle * mCbvSrvUavIncSize;

			return lGpuHandle;
		}
	}
}
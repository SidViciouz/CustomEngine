#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <memory>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <DirectXMath.h>
#include "../../Maths/header/Matrix4.h"
#include <functional>


using namespace Microsoft::WRL;
using namespace std;

#define MAX_ADAPTER_NUM 5
#define MAX_RESOURCE_NUM 1000
#define MAX_DESCRIPTOR_NUM 1000
#define MAX_RTV_NUM 5
#define MAX_DSV_NUM 10
#define MAX_OBJECT_NUM 1000
#define FRAMEDATA_NUM 3
#define SWAPCHAIN_BUFFER_NUM 2
#define MAX_PARTICLE_MEMORY_POOL 8
#define MAX_PARTICLE_NUM 10000
#define MAX_PARTICLE_EMITTER_NUM 100
#define MAX_PARTICLE_SYSTEM_NUM 100

struct SObjectData
{
	Math::SMatrix4								mWorldMatrix;
};

struct SWorldData
{
	Math::SMatrix4								mViewMatrix;
	Math::SMatrix4								mProjectionMatrix;
};
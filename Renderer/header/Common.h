#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <memory>
#include <array>
#include <string>
#include <unordered_map>

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

struct SObjectData
{

};

struct SWorldData
{

};
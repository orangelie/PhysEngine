#pragma once

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "gdiplus")
#pragma comment(lib, "dinput8")
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")
#if defined(_DEBUG)
#pragma comment(lib, "DirectXTex/DirectXTexd")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif

#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>
#include <Xinput.h>
#include <dinput.h>
#include <gdiplus.h>
#include <d2d1.h>
#include <dwrite.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "d3dx12.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTex.inl"

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>

#include "SimpleMath.h"
#include "SimpleMath.inl"
#include "resource.h"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

namespace fs = std::filesystem;
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;

using Vector2	= SimpleMath::Vector2;
using Vector3	= SimpleMath::Vector3;
using Vector4	= SimpleMath::Vector4;
using Matrix	= SimpleMath::Matrix;

#define HR(n) if(FAILED(n)) \
assert(-1); \

#define DEVICE GEngine->GetDevice()
#define GRAPHICS_CMDLIST GEngine->GetFrameBuffering()->GetCurrentCmdList()
#define RESOURCE_CMDLIST GEngine->GetFrameBuffering()->GetResourceCmdList()
#define FRAME_BUFFERING GEngine->GetFrameBuffering()

struct Resolution
{
	uint32 width;
	uint32 height;
};

struct TransformPass
{
	Matrix View;
	Matrix Proj;
	Matrix World;
	Vector3 EyePos;
	float pad;
};

struct Vertex
{
	Vertex() {}
	Vertex(
		const DirectX::XMFLOAT3& p,
		const DirectX::XMFLOAT3& n,
		const DirectX::XMFLOAT3& t,
		const DirectX::XMFLOAT2& uv) :
		Position(p),
		Normal(n),
		TangentU(t),
		TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TangentU(tx, ty, tz),
		TexC(u, v) {}

	Vector3 Position;
	Vector3 Normal;
	Vector3 TangentU;
	Vector2 TexC;
};

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint32>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,
	t5,

	END
};

enum
{
	CBV_REGISTER_COUNT = static_cast<uint32>(CBV_REGISTER::END),
	SRV_REGISTER_COUNT = static_cast<uint32>(SRV_REGISTER::END) - static_cast<uint32>(CBV_REGISTER::END),
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
};


extern unique_ptr<class Engine> GEngine;

void EnableConsole();
wstring Resources(const wchar_t* path);
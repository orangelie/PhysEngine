#include "pch.h"
#include "Shader.h"
#include "Engine.h"
#include "RootSignature.h"

void Shader::Init()
{
	_graphicsPiplineStateDesc = {};

	CreateVertexShader(L"../Resources/Shaders/default.hlsl", "VS_Main", "vs_5_0");
	CreatePixelShader(L"../Resources/Shaders/default.hlsl", "PS_Main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	_graphicsPiplineStateDesc.InputLayout = { desc, _countof(desc) };
	_graphicsPiplineStateDesc.pRootSignature = GEngine->GetRootSignature()->GetRootSignature().Get();
	_graphicsPiplineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_graphicsPiplineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_graphicsPiplineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_graphicsPiplineStateDesc.SampleMask = UINT_MAX;
	_graphicsPiplineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_graphicsPiplineStateDesc.NumRenderTargets = 1;
	_graphicsPiplineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_graphicsPiplineStateDesc.SampleDesc.Count = 1;
	_graphicsPiplineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	HR(DEVICE->CreateGraphicsPipelineState(&_graphicsPiplineStateDesc, IID_PPV_ARGS(&_piplineState)));
}

void Shader::CreateVertexShader(wstring path, string func, string target)
{
	CreateShader(path, func, target, _vsBlob, _graphicsPiplineStateDesc.VS);
}

void Shader::CreatePixelShader(wstring path, string func, string target)
{
	CreateShader(path, func, target, _psBlob, _graphicsPiplineStateDesc.PS);
}

void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	ComPtr<ID3DBlob> errBlob = nullptr;
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, &blob, &errBlob)))
	{
		if(errBlob)
			::MessageBoxA(nullptr, (char*)errBlob->GetBufferPointer(), "Shader Create Failed !", MB_OK);
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

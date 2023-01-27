#pragma once

class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	void Init();

	ComPtr<ID3D12PipelineState> GetPipelineState() const { return _piplineState; }

private:
	void CreateVertexShader(wstring path, string func, string target);
	void CreatePixelShader(wstring path, string func, string target);
	void CreateShader(const wstring& path, const string& func, const string& version,
		ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);

private:
	ComPtr<ID3D12PipelineState> _piplineState = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC _graphicsPiplineStateDesc = {};
	ComPtr<ID3DBlob> _vsBlob = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

};


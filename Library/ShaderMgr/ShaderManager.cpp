#include "ShaderManager.h"
#include <string>
#include <d3dx12.h>
#include "./Header/DirectXInit.h"

#include "./Header/Error.h"

ShaderManager::InputLayout::InputLayout() :
	inputLayout{}
{
}

void ShaderManager::InputLayout::PushInputLayout(LPCSTR SemanticName, DXGI_FORMAT Format)
{
	inputLayout.push_back({
		SemanticName,                               //セマンティック名
		0,                                          //同じセマンティック名が複数あるときに使うインデックス
		Format,                                     //要素数とビット数を表す
		0,                                          //入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,               //データのオフセット値
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, //入力データ種別
		0										    //一度に描画するインスタンス数
						  });
}

ShaderManager::PipelineState::PipelineState() :
	pipelinestate{},
	rootsignature(nullptr)
{
}

ShaderManager::BlendMode ShaderManager::blendMode = ShaderManager::BlendMode::NOBLEND;

ShaderManager::ShaderManager() :
	shaders{},
	inputLayouts{},
	gPipelines{},
	pipelines{}
{
}

ShaderManager::~ShaderManager()
{
}

ShaderManager* ShaderManager::Get()
{
	static ShaderManager instance = {};
	return &instance;
}

int ShaderManager::CreateShader(LPCWSTR vsFileName, LPCWSTR psFileName)
{
	shaders.push_back({});

	shaders.back().CompileVertexShader(vsFileName);
	shaders.back().CompilePixleShader(psFileName);

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateShader(Shaders::Shader veatex, LPCWSTR psFileName)
{
	shaders.push_back({});

	shaders.back().GetVertex() = veatex;
	shaders.back().CompilePixleShader(psFileName);

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateShader(LPCWSTR vsFileName, Shaders::Shader pixle)
{
	shaders.push_back({});

	shaders.back().CompileVertexShader(vsFileName);
	shaders.back().GetPixle() = pixle;

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateShader(Shaders::Shader veatex, Shaders::Shader pixle)
{
	shaders.push_back({});

	shaders.back().GetVertex() = veatex;
	shaders.back().GetPixle() = pixle;

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateGPipeline(const int& inputLayoutIndex, const int& shaderIndex)
{
	for (size_t i = 0; i < sizeof(gPipelines) / sizeof(gPipelines[0]); i++)
	{
		gPipelines[i].push_back({});
		auto& gpipeline = gPipelines[i].back();

		// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaders[shaderIndex].GetVertex().GetShaderBlob());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaders[shaderIndex].GetPixle().GetShaderBlob());
		// サンプルマスクとラスタライザステートの設定
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //背面カリングしない
		// ブレンドデスクの設定
		D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = gpipeline.BlendState.RenderTarget[0];
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //標準設定

		switch (i)
		{
		case BlendMode::NOBLEND:
			/*ノーブレンド用の設定*/
			blendDesc.BlendEnable = false; //ブレンドを無効にする
			break;
		case BlendMode::ALPHA:
			/*αブレンド用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;          //加算
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;      //ソースのアルファ値
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f - ソースのアルファ値
			goto BaseBlendState;
		case BlendMode::ADD:
			/*加算合成用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD; //加算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;   //ソースの値を 100% 使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;  //デストの値を 100% 使う
			goto BaseBlendState;
		case BlendMode::SUB:
			/*減算合成用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; //デストからソースを減算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;            //ソースの値を 100% 使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;           //デストの値を 100% 使う
			goto BaseBlendState;
		case BlendMode::INV:
			/*色反転用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;          //加算
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; //1.0f - デストカラーの値
			blendDesc.DestBlend = D3D12_BLEND_ZERO;          //使わない
			goto BaseBlendState;
		default:
		BaseBlendState:
			/*共通設定*/
			blendDesc.BlendEnable = true;                //ブレンドを有効にする
			blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //加算
			blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;   //ソースの値を 100% 使う
			blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; //デストの値を   0% 使う
			break;
		}
		// デプスステンシルステートの設定
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
		// 頂点レイアウトの設定
		gpipeline.InputLayout = inputLayouts[inputLayoutIndex].GetInputLayout();
		// 図形の形状を三角形に設定
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		// その他の設定
		gpipeline.NumRenderTargets = 1; //描画対象は1つ
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0~255指定のRGBA
		gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	}

	return static_cast<int>(gPipelines[0].size() - 1);
}

int ShaderManager::CreatePipeline(const int& inputLayoutIndex, const int& shaderIndex)
{
	if (inputLayoutIndex < 0 || shaderIndex< 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	HRESULT hr = S_FALSE;
	ComPtr<ID3DBlob> rootSigBlob = {};
	ComPtr<ID3DBlob> errorBlob = {};

	pipelines.push_back({});
	auto& pipeline = pipelines.back();
	auto dev = DirectXInit::GetDevice();

	// デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {}; //デスクリプタテーブルの設定(シェーダリソース)
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[2]{}; //ルートパラメータの設定
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(
		_countof(rootparams),
		rootparams,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	hr = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob
	);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipeline.rootsignature)
	);
	if (FAILED(hr))
	{
		return hr;
	}

	for (size_t j = 0; j < sizeof(PipelineState::pipelinestate) / sizeof(PipelineState::pipelinestate[0]); j++)
	{
		// グラフィックスパイプライン設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};

		// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaders[shaderIndex].GetVertex().GetShaderBlob());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaders[shaderIndex].GetPixle().GetShaderBlob());
		// サンプルマスクとラスタライザステートの設定
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //背面カリングしない
		// ブレンドデスクの設定
		D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = gpipeline.BlendState.RenderTarget[0];
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //標準設定

		switch (j)
		{
		case BlendMode::NOBLEND:
			/*ノーブレンド用の設定*/
			blendDesc.BlendEnable = false; //ブレンドを無効にする
			break;
		case BlendMode::ALPHA:
			/*αブレンド用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;          //加算
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;      //ソースのアルファ値
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f - ソースのアルファ値
			goto BaseBlendState;
		case BlendMode::ADD:
			/*加算合成用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD; //加算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;   //ソースの値を 100% 使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;  //デストの値を 100% 使う
			goto BaseBlendState;
		case BlendMode::SUB:
			/*減算合成用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; //デストからソースを減算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;            //ソースの値を 100% 使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;           //デストの値を 100% 使う
			goto BaseBlendState;
		case BlendMode::INV:
			/*色反転用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;          //加算
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; //1.0f - デストカラーの値
			blendDesc.DestBlend = D3D12_BLEND_ZERO;          //使わない
			goto BaseBlendState;
		default:
		BaseBlendState:
			/*共通設定*/
			blendDesc.BlendEnable = true;                //ブレンドを有効にする
			blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //加算
			blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;   //ソースの値を 100% 使う
			blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; //デストの値を   0% 使う
			break;
		}
		// デプスステンシルステートの設定
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
		// 頂点レイアウトの設定
		gpipeline.InputLayout = inputLayouts[inputLayoutIndex].GetInputLayout();
		// 図形の形状を三角形に設定
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		// その他の設定
		gpipeline.NumRenderTargets = 1; //描画対象は1つ
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0~255指定のRGBA
		gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング

		// パイプラインにルートシグネチャをセット
		gpipeline.pRootSignature = pipeline.rootsignature.Get();

		hr = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline.pipelinestate[j]));
	}

	return static_cast<int>(pipelines.size() - 1);
}

#include "./Header/ShaderManager.h"
#include <string>
#include <d3dx12.h>
#include "./Header/DirectXInit.h"

#include "./Header/Error.h"

// シェーダーコンパイル用
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr<ID3D10Blob> ShaderManager::Shader::errorBlob = {};

ShaderManager::Shader::Shader(LPCWSTR pFileName, ShaderType shaderType)
{
	static LPCSTR pTarget = nullptr;

	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		pTarget = "vs_5_0";
		break;
	case ShaderType::PIXLE_SHADER:
		pTarget = "ps_5_0";
		break;
	case ShaderType::DOMAIN_SHADER:
		pTarget = "ds_5_0";
		break;
	case ShaderType::HULL_SHADER:
		pTarget = "hs_5_0";
		break;
	case ShaderType::GEOMETRY_SHADER:
		pTarget = "gs_5_0";
		break;
	case ShaderType::COMPUTE_SHADER:
		pTarget = "cs_5_0";
		break;
	default:
		shaderBlob = nullptr;
		errorBlob = nullptr;
		return;
		break;
	}

	HRESULT hr = D3DCompileFromFile(
		pFileName, //シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				 //インクルード可能にする
		"main",											 //エントリーポイント名
		pTarget,										 //シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0,
		&shaderBlob,
		&errorBlob);
	if (FAILED(hr))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

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
	vertexShaders{},
	pixleShaders{},
	domainShaders{},
	hullShaders{},
	geometryShaders{},
	computeShaders{},
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

int ShaderManager::CompileVertexShader(LPCWSTR pFileName)
{
	vertexShaders.push_back(Shader(pFileName, ShaderType::VERTEX_SHADER));
	return static_cast<int>(vertexShaders.size() - 1);
}

int ShaderManager::CompilePixleShader(LPCWSTR pFileName)
{
	pixleShaders.push_back(Shader(pFileName, ShaderType::PIXLE_SHADER));
	return static_cast<int>(pixleShaders.size() - 1);
}

int ShaderManager::CompileDomainShader(LPCWSTR pFileName)
{
	domainShaders.push_back(Shader(pFileName, ShaderType::DOMAIN_SHADER));
	return static_cast<int>(domainShaders.size() - 1);
}

int ShaderManager::CompileHullShader(LPCWSTR pFileName)
{
	hullShaders.push_back(Shader(pFileName, ShaderType::HULL_SHADER));
	return static_cast<int>(hullShaders.size() - 1);
}

int ShaderManager::CompileGeometryShader(LPCWSTR pFileName)
{
	geometryShaders.push_back(Shader(pFileName, ShaderType::GEOMETRY_SHADER));
	return static_cast<int>(geometryShaders.size() - 1);
}

int ShaderManager::CompileComputeShader(LPCWSTR pFileName)
{
	computeShaders.push_back(Shader(pFileName, ShaderType::COMPUTE_SHADER));
	return static_cast<int>(computeShaders.size() - 1);
}

int ShaderManager::CreateGPipeline(const int& inputLayoutIndex, const int& vsIndex, const int& psIndex)
{
	for (size_t i = 0; i < sizeof(gPipelines) / sizeof(gPipelines[0]); i++)
	{
		gPipelines[i].push_back({});
		auto& gpipeline = gPipelines[i].back();

		// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(vertexShaders[vsIndex].GetShaderBlob());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(pixleShaders[psIndex].GetShaderBlob());
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

int ShaderManager::CreatePipeline(const int& inputLayoutIndex, const int& vsIndex, const int& psIndex)
{
	if (inputLayoutIndex < 0 || vsIndex< 0 || psIndex< 0)
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
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(vertexShaders[vsIndex].GetShaderBlob());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(pixleShaders[psIndex].GetShaderBlob());
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

#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Shaders.h"
#include <vector>

class ShaderManager final
{
public: //シングルトン化
	static ShaderManager* Get();
private:
	ShaderManager();
	ShaderManager(const ShaderManager&) = delete;
	~ShaderManager();
	ShaderManager operator=(const ShaderManager&) = delete;

private: //エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template<class T> using vector = std::vector<T>;

public: //サブクラス
	class InputLayout
	{
	public: //メンバ変数
		vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	public: //メンバ関数
		InputLayout();
		~InputLayout() = default;

		D3D12_INPUT_LAYOUT_DESC GetInputLayout() const { return  { inputLayout.data(), static_cast<UINT>(inputLayout.size()) }; }
		void PushInputLayout(LPCSTR SemanticName, DXGI_FORMAT Format);
		void PopInputLayout() { inputLayout.pop_back(); }
	};

	enum BlendMode
	{
		NOBLEND, //ノーブレンド
		ALPHA,	 //αブレンド
		ADD,	 //加算合成
		SUB,	 //減算合成
		INV		 //色反転合成
	};

	struct PipelineState
	{
	public: //メンバ変数
		ComPtr<ID3D12PipelineState> pipelinestate[5]; //パイプラインの状態
		ComPtr<ID3D12RootSignature> rootsignature;    //ルートシグネチャ

	public: //メンバ関数
		// コンストラクタ
		PipelineState();
	};

public: //静的メンバ変数
	static BlendMode blendMode;

private: //メンバ変数
	vector<Shaders> shaders; //シェーダー群

	vector<InputLayout> inputLayouts; //インプットレイアウト
	vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC> gPipelines[5]; //グラフィックパイプライン
	vector<PipelineState> pipelines; //パイプライン

public: //メンバ関数
	// コンパイル済シェーダーデータの生成
	int CreateShader(LPCWSTR vsFileName, LPCWSTR psFileName);
	int CreateShader(Shaders::Shader veatex, LPCWSTR psFileName);
	int CreateShader(LPCWSTR vsFileName, Shaders::Shader pixle);
	int CreateShader(Shaders::Shader veatex, Shaders::Shader pixle);

	// インプットレイアウトの生成
	int CreateInputLayout() { inputLayouts.push_back({}); return static_cast<int>(inputLayouts.size() - 1); }
	// グラフィックパイプラインの生成
	int CreateGPipeline(const int& inputLayoutIndex, const int& shaderIndex);
	// パイプラインの生成
	int CreatePipeline(const int& inputLayoutIndex, const int& shaderIndex);

	Shaders GetShader(const int& index) const { return shaders.at(index); }
	InputLayout& GetInputLayout(const int& index) { return inputLayouts.at(index); }
	PipelineState GetPipeline(const int& index) const { return pipelines.at(index); }
};

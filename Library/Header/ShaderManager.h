#pragma once
#include <d3d12.h>
#include <wrl.h>
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
	enum ShaderType
	{
		VERTEX_SHADER,   //頂点シェーダー
		PIXLE_SHADER,    //ピクセルシェーダー
		DOMAIN_SHADER,   //ドメインシェーダー
		HULL_SHADER,     //ハルシェーダー
		GEOMETRY_SHADER, //ジオメトリシェーダー
		COMPUTE_SHADER   //コンピュートシェーダー
	};

	class Shader
	{
	private: //静的メンバ変数
		static ComPtr<ID3DBlob> errorBlob;
		
	private: //メンバ変数
		ComPtr<ID3DBlob> shaderBlob;

	public: //メンバ関数
		Shader(LPCWSTR pFileName, ShaderType shaderType);
		~Shader() = default;

		ID3DBlob* GetShaderBlob() const { return shaderBlob.Get(); }
	};

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
	vector<Shader> vertexShaders;   //頂点シェーダー
	vector<Shader> pixleShaders;    //ピクセルシェーダー
	vector<Shader> domainShaders;   //ドメインシェーダー
	vector<Shader> hullShaders;     //ハルシェーダー
	vector<Shader> geometryShaders; //ジオメトリシェーダー
	vector<Shader> computeShaders;  //コンピュートシェーダー

	vector<InputLayout> inputLayouts; //インプットレイアウト
	vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC> gPipelines[5]; //グラフィックパイプライン
	vector<PipelineState> pipelines; //パイプライン

public: //メンバ関数
	// 頂点シェーダーのコンパイル
	int CompileVertexShader(LPCWSTR pFileName);
	// ピクセルシェーダーのコンパイル
	int CompilePixleShader(LPCWSTR pFileName);
	// ドメインシェーダーのコンパイル
	int CompileDomainShader(LPCWSTR pFileName);
	// ハルシェーダーのコンパイル
	int CompileHullShader(LPCWSTR pFileName);
	// ジオメトリシェーダーのコンパイル
	int CompileGeometryShader(LPCWSTR pFileName);
	// コンピュートシェーダーのコンパイル
	int CompileComputeShader(LPCWSTR pFileName);

	// インプットレイアウトの生成
	int CreateInputLayout() { inputLayouts.push_back({}); return static_cast<int>(inputLayouts.size() - 1); }
	// グラフィックパイプラインの生成
	int CreateGPipeline(const int& inputLayoutIndex, const int& vsIndex, const int& psIndex);
	// パイプラインの生成
	int CreatePipeline(const int& inputLayoutIndex, const int& vsIndex, const int& psIndex);

	Shader GetVertexShader(const int& index) { return vertexShaders.at(index); }
	Shader GetPixleShader(const int& index) { return pixleShaders.at(index); }
	Shader GetDomainShader(const int& index) { return domainShaders.at(index); }
	Shader GetHullShader(const int& index) { return hullShaders.at(index); }
	Shader GetGeometryShader(const int& index) { return geometryShaders.at(index); }
	Shader GetComputeShader(const int& index) { return computeShaders.at(index); }
	InputLayout& GetInputLayout(const int& index) { return inputLayouts.at(index); }
	PipelineState GetPipeline(const int& index) const { return pipelines.at(index); }
};

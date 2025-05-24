#pragma once
#include "AllSetting.h"

// 画面描画関係
class GraphicsManager
{
public:

	/// <summary>
	/// グラフィックスパイプラインの初期化
	/// </summary>
	static void InitGraphics();

	/// <summary>
	/// 描画
	/// </summary>
	static void Render();

	/// <summary>
	/// グラフィックスのリリース
	/// </summary>
	static void ReleaseGraphics();

private:

	/// <summary>
	/// サイズに合わせてアライメントする
	/// </summary>
	/// <param name="size">アライメントしたいサイズ</param>
	/// <param name="alignment">アライメント倍数</param>
	/// <returns>アライメント後サイズ</returns>
	inline static ULONGLONG AligmentedSize(ULONGLONG size, ULONGLONG alignment) {
		return (size + alignment - 1) & ~(alignment - 1);
	}

	// パイプラインステートオブジェクト
	static ID3D12PipelineState* _pipelinestate;

	// ルートシグネチャ
	static ID3D12RootSignature* _rootsignature;

	// 頂点シェーダーバイナリ
	static ID3DBlob* _vsBlob;

	// ピクセルシェーダーバイナリ
	static ID3DBlob* _psBlob;

	// エラーバイナリ
	static ID3DBlob* _errorBlob;

	// ビューポート
	static D3D12_VIEWPORT _viewport;

	// シザー矩形
	static D3D12_RECT _scissorrect;
};


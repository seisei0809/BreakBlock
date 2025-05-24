#pragma once
#include "AllSetting.h"

// 定数バッファに渡すデータ
struct ConstBufferData {

	XMMATRIX mat;   // 行列4x4
	XMFLOAT4 color; // RGBAの色情報
};

// 位置回転スケーリングを持つ
struct Transform {

	// 位置座標
	XMFLOAT3 position;

	// 回転座標
	FLOAT rotation;

	// スケーリング座標
	XMFLOAT2 scale;
};

// Sprite本体
class Sprite
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="path">パス</param>
	/// <param name="fence">専用のフェンスがあるなら</param>
	/// <param name="fenceVal">専用のフェンス完了数のエイリアス</param>
	Sprite(WSTR, ID3D12Fence* = nullptr, UINT64 & = fenceValDefault);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();

	/// <summary>
	/// ゲーム終了時にキャッシュ解放用
	/// </summary>
	static void CacheDelete();

	/// <summary>
	/// Transform要素を直に渡す
	/// </summary>
	/// <returns>Transform本体</returns>
	inline Transform& GetTransform() { return transform; }

	/// <summary>
	/// Color要素を直に渡す
	/// </summary>
	/// <returns></returns>
	inline XMFLOAT4& GetColor() { return color; }

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画してもいいかのチェック
	/// </summary>
	/// <returns>アクティブならtrue</returns>
	inline BOOL GetActive() const { return isActive; }

	/// <summary>
	/// 描画するかどうかの変更
	/// </summary>
	/// <param name="active">アクティブ状態</param>
	inline void SetActive(BOOL active) { isActive = active; }

private:

	// アライメントの計算に必要
	ULONGLONG AligmentedSize(ULONGLONG size, ULONGLONG alignment);

	// 位置回転スケーリングを持ったTransform
	Transform transform;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;

	// 頂点バッファ
	ID3D12Resource* vertBuff;

	// 頂点バッファアップロード用
	ID3D12Resource* uploadBuff;

	// インデックスバッファ
	ID3D12Resource* idxBuff;

	// テクスチャバッファ
	ID3D12Resource* texBuff;

	// 定数バッファ
	ID3D12Resource* constBuff;

	// SRVとCBVを指すディスクリプタヒープ
	ID3D12DescriptorHeap* descriHeap;

	// アップロード用アロケータ
	ID3D12CommandAllocator* uploadAllocator;

	// アップロード用コマンドリスト
	ID3D12GraphicsCommandList* uploadCmdList;

	// アップロード用コマンドキュー
	ID3D12CommandQueue* uploadCmdQueue;

	// 専用フェンス
	ID3D12Fence* fence;

	// 専用のフェンスの完了数
	UINT64& fenceVal;

	// デフォルト引数用
	static UINT64 fenceValDefault;

	// 色情報
	XMFLOAT4 color;

	// 描画してもいいか
	BOOL isActive;

	// 画像の名前
	WSTR imageName;

	// 重複防止のためのテクスチャキャッシュ
	static std::map<std::wstring, ID3D12Resource*> texBuffCache;

	// 重複防止のためのメタデータキャッシュ
	static std::map<std::wstring, TexMetadata> metadataCache;

	// 重複防止のための画像本体キャッシュ
	static std::map<std::wstring, ScratchImage> scratchImgCache;
};


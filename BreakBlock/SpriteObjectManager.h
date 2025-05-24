#pragma once
#include "AllSetting.h"
#include "Sprite.h"

// 頂点情報
struct Vertex {

	XMFLOAT3 position;  // 頂点位置
	XMFLOAT2 uv;        // uv座標
};

// 定数バッファのデータとしてシェーダーに送る構造体
struct ConstantBufferData {

	XMMATRIX mat;   // 座標変換用行列
	XMFLOAT4 color; // RGBAの色情報
};

// 全てのSprite管理
class SpriteObjectManager {
public:

	/// <summary>
	/// Spriteを配列に追加する
	/// </summary>
	/// <param name="">Sprite本体</param>
	static void AddSprite(Sprite*);

	/// <summary>
	/// Spriteを破棄する
	/// </summary>
	/// <param name="sprite">Sprite本体</param>
	static void DeleteSprite(Sprite*);

	/// <summary>
	/// SpriteごとのDrawを順に呼び出していく
	/// </summary>
	static void Draw();

	/// <summary>
	/// Spriteを破棄する
	/// </summary>
	static void AllDeleteSprite();

private:

	// Sprite保存配列
	static VECTOR<Sprite*> _sprites;
};


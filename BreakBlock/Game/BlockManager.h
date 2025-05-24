#pragma once
#include "../Object.h"
#include "RainbowColor.h"

// ブロック管理
class BlockManager : public Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BlockManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BlockManager();

	/// <summary>
	/// 最初に呼び出される関数
	/// </summary>
	/// <param name="">シーン本体</param>
	virtual void Start(Scene*);

	/// <summary>
	/// 毎フレーム呼び出される関数
	/// </summary>
	/// <param name="">シーン本体</param>
	virtual void Update(Scene*);

	/// <summary>
	/// ブロック配列を取得する
	/// </summary>
	/// <returns>ブロック本体の配列</returns>
	inline VECTOR<Sprite*> GetBlocks() { return blocks; }

	/// <summary>
	/// 全てが非アクティブになったかどうかを取得する
	/// </summary>
	/// <returns>全て非アクティブならtrue</returns>
	inline BOOL GetAllNoActive() const { return isAllNoActive; }
	
private:

	// 配置するブロックの配列
	VECTOR<Sprite*> blocks;

	// 画像の名前
	const WSTR imageName = L"BallAndBlock.png";

	// ブロックのサイズ
	const XMFLOAT2 size = { 60.0f,10.0f };

	// 配置するブロックの初期位置
	const XMFLOAT3 initPos = { 120.0f,20.0f,1.0f };

	// 配置する間隔
	const XMFLOAT2 space = { 140.0f,30.0f };

	// 行と列何個置くか
	const XMINT2 setMat = { 5,10 };

	// ブロックをレインボー変化させる
	RainbowColor* rainbow;

	// レインボー変化の速度
	const FLOAT rainbowSpeed = 10.0f;

	// 全てが非アクティブになったかどうか
	BOOL isAllNoActive;
};


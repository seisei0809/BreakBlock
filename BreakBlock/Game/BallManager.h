#pragma once
#include "../Object.h"
#include "AudioPlayer.h"
#include "RainbowColor.h"

class BallManager : public Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BallManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BallManager();

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
	/// 今動いてるか
	/// </summary>
	/// <returns>動いてるならtrue</returns>
	inline BOOL GetNowMove() const { return isMove; }

private:

	/// <summary>
	/// 画面内の何かに衝突したのを検知
	/// </summary>
	/// <param name="someTr">見るTransform</param>
	/// <param name="mulOffset">オフセットは何倍させるか</param>
	/// <param name="isBar">バーかどうか</param>
	/// <returns>衝突したかどうか</returns>
	BOOL IsCollision(Transform, XMFLOAT2, BOOL);

	// ボール
	Sprite* ball;

	// 画像の名前
	const WSTR imageName = L"BallAndBlock.png";

	// ボールのサイズ
	const XMFLOAT2 size = { 20.0f,20.0f };

	// 配置するボールの初期位置
	const XMFLOAT3 initPos = { (FLOAT)(window_width) / 2.0f,(FLOAT)(window_height)-130.0f ,3.0f };

	// 速度
	XMFLOAT2 speed;

	// 初期の速度
	const XMFLOAT2 initSpeed = { 600.0f,600.0f };

	// 最終的に何倍の速度を足すか
	const FLOAT addLastSpeed = 1.5f;

	// 回転速度
	const FLOAT rotSpeed = 30.0f;

	// 0～1を受け取る
	FLOAT delta;

	// 衝突位置をオフセット
	const XMFLOAT2 collisionOffset = { 25.0f,5.0f };

	// 最初のブロックの数
	UINT64 initBlockValue;

	// 動けるかどうか
	BOOL isMove;

	// ゲームクリアしたかどうか
	BOOL isClear;

	// 一回でもアウトしたか
	BOOL isOut;

	// ゲーム終了時に一回もアウトしないならこの目標値にするため
	const Transform targetTr = { {window_width / 2.0f,window_height / 2.0f - 100.0f,2.0f},0.0f,{200.0f,200.0f} };

	// 目標値とどれだけ差があるか
	Transform targetDif;

	// クリア時最後のTransform状態
	Transform lastTr;

	// 線形的に何秒で目標のTransformまで持っていくか
	const FLOAT lerpTime = 1.0f;

	// 経過時間
	FLOAT elapsedTime;

	// 消すときの音
	AudioPlayer* breakSE;

	// 音のパス
	const WSTR audioPath = L"BreakSE.wav";

	// クリア時にレインボーにする
	RainbowColor* rainbow;

	// レインボーの速さ
	const FLOAT rainbowSpeed = 20.0f;
};


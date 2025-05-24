#pragma once
#include "../Object.h"
#include "RainbowColor.h"

class SpaceKeyText : public Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpaceKeyText();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SpaceKeyText();

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

private:

	// スペースキーを促す
	Sprite* spaceClick;

	// 画像のパス
	const WSTR imagePath = L"SpaceClickImage.png";

	// どっちにフェードしてるか
	BOOL isUpFade;

	// フェード速度
	const FLOAT fadeSpeed = 5.0f;

	// 最初の位置
	const XMFLOAT3 initPos = { (FLOAT)window_width / 2.0f,(FLOAT)window_height / 2.0f + 120.0f,1 };

	// 大きさ
	const XMFLOAT2 size = { 200.0f,200.0f };

	// レインボー用
	RainbowColor* rainbow;

	// レインボーの速さ
	const FLOAT rainbowSpeed = 20.0f;
};


#pragma once
#include "../Object.h"

// ローディング管理
class LoadingManager : public Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoadingManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LoadingManager();

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

	// ローディングに出す文字
	Sprite* loadingText;

	// ローディング文字の名前
	const WSTR loadingTextName = L"LoadingImage.png";

	// ローディング文字のTransform
	const Transform loadingTextTr =
	{ {(FLOAT)window_width - 150.0f,(FLOAT)window_height - 30.0f,2.0f},0.0f,{150.0f,150.0f} };

	// 完了した時に出す文字
	Sprite* completeText;

	// 完了文字の名前
	const WSTR completeTextName = L"CompleteImage.png";

	// 完了文字のTransform
	const Transform completeTextTr =
	{ {(FLOAT)window_width - 150.0f,(FLOAT)window_height - 30.0f,1.0f},0.0f,{150.0f,150.0f} };

	// ローディング中に出すボール
	Sprite* loadingBall;

	// ローディングボールの名前
	const WSTR loadingBallName = L"BallAndBlock.png";

	// ボールの回転スピード
	const FLOAT ballRotation = 10.0f;

	// ローディングボールのTransform
	const Transform loadingBallTr =
	{ {(FLOAT)window_width - 20.0f,(FLOAT)window_height - 30.0f,1.0f},0.0f,{20.0f,20.0f} };

	// ローディングする画像の名前
	const WSTR loadingImagesName[4] =
	{ L"Bar.png",L"ESCImage.png",L"SpaceClickImage.png",L"TitleImage.png" };

	// ローディング専用フェンス
	ID3D12Fence* fence;

	// 専用のフェンスの完了数
	UINT64 fenceVal;

	// 完了まで最低何秒のdelayにするか(ちらつき防止)
	const FLOAT delayTime = 1.0f;

	// 経過時間
	FLOAT elapsedTime;

	// 移行前の背景色
	FLOAT lastColor[4];

	// 完了後の背景色
	const FLOAT completeColor[4] = { 1.0f,1.0f,0.0f,1.0f };

	// 移行前と完了後がどれだけ違うか
	FLOAT difColor[4];

	// 完了後何秒で遷移するか
	const FLOAT completedTime = 1.0f;

	// 線形移行中か
	BOOL isLerp;

	// 何枚目のロードか
	INT loadIndex;
};


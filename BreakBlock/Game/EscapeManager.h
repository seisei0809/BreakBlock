#pragma once
#include "../Object.h"
class EscapeManager : public Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EscapeManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EscapeManager();

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

	// エスケープキーを促す
	Sprite* escapeClick;

	// 画像のパス
	const WSTR imagePath = L"ESCImage.png";

	// 初期位置
	const XMFLOAT3 initPos = { 25.0f,25.0f,3.0f };

	// サイズ
	const XMFLOAT2 size = { 20.0f,20.0f };
};


#pragma once
#include "../AllSetting.h"

// 虹色変化させる
class RainbowColor {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="speed">速度</param>
	RainbowColor(float);

	/// <summary>
	/// 呼び出すと色を変化させる
	/// </summary>
	void Update();

	/// <summary>
	/// 色を取得する
	/// </summary>
	/// <returns>現在の色</returns>
	inline XMFLOAT4 GetColor() { return { r, g, b, 1.0f }; }

private:

	// 赤
	FLOAT r;
	// 緑
	FLOAT g;
	// 青
	FLOAT b;
	// 変化速度
	FLOAT speed;
	// 状態
	INT state;

};


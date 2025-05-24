#pragma once
#include "../Object.h"

class BarManager : public Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BarManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BarManager();

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
	/// バーを取得する
	/// </summary>
	/// <returns>バーの本体</returns>
	inline Sprite* GetBar() { return bar; }

private:

	// バー
	Sprite* bar;

	// バーのサイズ
	const XMFLOAT2 size = { 70.0f,30.0f };

	// 配置するバーの初期位置
	const XMFLOAT3 initPos = { (FLOAT)(window_width) / 2.0f,(FLOAT)window_height - 60.0f ,1.0f };

	// 動くスピード
	const FLOAT moveSpeed = 1500;
};


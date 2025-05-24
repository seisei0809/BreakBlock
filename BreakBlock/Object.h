#pragma once
#include "SpriteObjectManager.h"
#include "AllSetting.h"

// 前方宣言

class Scene;

// オブジェクト基底
class Object {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object() = default;

	/// <summary>
	/// 最初に呼び出される関数
	/// </summary>
	/// <param name="">シーン本体</param>
	virtual void Start(Scene*) = 0;

	/// <summary>
	/// 毎フレーム呼び出される関数
	/// </summary>
	/// <param name="">シーン本体</param>
	virtual void Update(Scene*) = 0;
};

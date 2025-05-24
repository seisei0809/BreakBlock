#pragma once
#include "../Scene.h"
#include "LoadingManager.h"

// 最初のローディングシーン
class LoadingScene : public Scene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoadingScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LoadingScene();
};


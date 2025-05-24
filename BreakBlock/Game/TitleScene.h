#pragma once
#include "../Scene.h"
#include "AudioPlayer.h"
#include "SpaceKeyText.h"
#include "EscapeManager.h"

// タイトルシーン
class TitleScene : public Scene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TitleScene();

private:

	// BGM流す用
	AudioPlayer* bgm;

	// タイトル文字
	Sprite* titleText;
};


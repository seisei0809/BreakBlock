#include "../AllSetting.h"
#include "TitleScene.h"

TitleScene::TitleScene() :
	Scene() {

	try
	{
		// Objectを定義
		AddObject<SpaceKeyText>();
		AddObject<EscapeManager>();

		// bgm流す
		bgm = new AudioPlayer(L"TitleBGM.wav");
		bgm->Play(true);

		// 何も処理しないSpriteはここ
		titleText = new Sprite(L"TitleImage.png");
		Transform& tr = titleText->GetTransform();
		tr.position = { (FLOAT)window_width / 2.0f, (FLOAT)window_height / 2.0f,1 };
		tr.scale = { 300.0f,300.0f };
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("タイトルシーンコンストラクタで異常エラー") + STR(e.what()) + STR(":"));
	}
}

TitleScene::~TitleScene() {

	if (bgm)		delete bgm;
}
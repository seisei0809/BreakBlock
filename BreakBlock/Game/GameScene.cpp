#include "../AllSetting.h"
#include "GameScene.h"

GameScene::GameScene() :
	Scene() {

	try
	{
		// Objectの配置
		blockManager = AddObject<BlockManager>();
		ballManager = AddObject<BallManager>();
		barManager = AddObject<BarManager>();
		AddObject<SpaceKeyText>();
		AddObject<EscapeManager>();

		// bgm流す
		bgm = new AudioPlayer(L"GameBGM.wav");
		bgm->Play(true);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ゲームシーンコンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

GameScene::~GameScene() {

	if (bgm)	 delete bgm;
}
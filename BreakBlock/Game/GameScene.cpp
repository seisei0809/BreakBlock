#include "../AllSetting.h"
#include "GameScene.h"

GameScene::GameScene() :
	Scene() {

	try
	{
		// Object�̔z�u
		blockManager = AddObject<BlockManager>();
		ballManager = AddObject<BallManager>();
		barManager = AddObject<BarManager>();
		AddObject<SpaceKeyText>();
		AddObject<EscapeManager>();

		// bgm����
		bgm = new AudioPlayer(L"GameBGM.wav");
		bgm->Play(true);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�Q�[���V�[���R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

GameScene::~GameScene() {

	if (bgm)	 delete bgm;
}
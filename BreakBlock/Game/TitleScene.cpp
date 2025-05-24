#include "../AllSetting.h"
#include "TitleScene.h"

TitleScene::TitleScene() :
	Scene() {

	try
	{
		// Object���`
		AddObject<SpaceKeyText>();
		AddObject<EscapeManager>();

		// bgm����
		bgm = new AudioPlayer(L"TitleBGM.wav");
		bgm->Play(true);

		// �����������Ȃ�Sprite�͂���
		titleText = new Sprite(L"TitleImage.png");
		Transform& tr = titleText->GetTransform();
		tr.position = { (FLOAT)window_width / 2.0f, (FLOAT)window_height / 2.0f,1 };
		tr.scale = { 300.0f,300.0f };
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�^�C�g���V�[���R���X�g���N�^�ňُ�G���[") + STR(e.what()) + STR(":"));
	}
}

TitleScene::~TitleScene() {

	if (bgm)		delete bgm;
}
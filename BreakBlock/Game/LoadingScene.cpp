#include "../AllSetting.h"
#include "LoadingScene.h"

LoadingScene::LoadingScene() :
	Scene() {

	try
	{
		AddObject<LoadingManager>();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���[�f�B���O�Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

LoadingScene::~LoadingScene() {


}
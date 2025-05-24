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
		throw std::runtime_error(STR("ローディング管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

LoadingScene::~LoadingScene() {


}
#include "AllSetting.h"
#include "Scene.h"
#include "SceneManager.h"

Scene::Scene() {

	try
	{
		// とりあえず全部消す
		SpriteObjectManager::AllDeleteSprite();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("シーンコンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

Scene::~Scene() {

	// 全てのObjectを解放
	for (auto obj : objects) {

		delete obj;
	}
	// クリア
	objects.clear();
}

void Scene::Start() {

	try
	{
		// 全てのStartメソッドを呼び出す
		for (auto obj : objects) {

			obj->Start(this);
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("シーンStartで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void Scene::Update() {

	try
	{
		// 全てのUpdateメソッドを呼び出す
		for (auto obj : objects) {

			// シーンの変更を検知してまだやる気なら今すぐ抜ける
			if (SceneManager::GetChangeNow())break;

			obj->Update(this);
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("シーンUpdateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}
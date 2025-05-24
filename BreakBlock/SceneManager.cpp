#include "AllSetting.h"
#include "SceneManager.h"

// staticメンバの実体を定義

Scene* SceneManager::_currentScene = nullptr;
BOOL SceneManager::_isChangeNow = false;

void SceneManager::ChangeScene(Scene* newScene) {

	try
	{
		// 現在のシーンがあるなら
		if (_currentScene) {

			// 破棄
			delete _currentScene;
		}
		// シーンを変更するとする
		_isChangeNow = true;

		// 新しいシーンの設定
		_currentScene = newScene;

		// Startをとりあえず呼ぶ
		_currentScene->Start();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("シーン管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void SceneManager::DeleteScene() {

	try
	{
		if (_currentScene)   delete _currentScene;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("シーン管理シーン破棄で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

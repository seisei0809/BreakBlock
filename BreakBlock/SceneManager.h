#pragma once
#include "Scene.h"

// シーン管理
class SceneManager {
public:

	/// <summary>
	/// シーンを変更する
	/// </summary>
	/// <param name="newScene">新しいシーン</param>
	static void ChangeScene(Scene*);

	/// <summary>
	/// 現在のシーンを取得する
	/// </summary>
	/// <returns>現在のシーン本体</returns>
	inline static Scene* GetCurrentScene() { return _currentScene; }

	/// <summary>
	/// シーンを破棄する
	/// </summary>
	static void DeleteScene();

	/// <summary>
	/// 現在シーンを変更しようとしているか
	/// </summary>
	/// <returns>しようとしてるならtrue</returns>
	inline static BOOL& GetChangeNow() { return _isChangeNow; }

private:

	// 現在のシーン
	static Scene* _currentScene;

	// 現在シーンを変更しようとしているか
	static BOOL _isChangeNow;
};

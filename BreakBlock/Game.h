#pragma once
#include "AllSetting.h"
#include "Debugger.h"
#include "DeviceManager.h"
#include "GraphicsManager.h"
#include "SpriteObjectManager.h"
#include "SceneManager.h"

// ゲームの根幹
class Game {
public:

	/// <summary>
	/// ゲームを開始する
	/// </summary>
	static void GameStart();

	/// <summary>
	/// ゲームを終わらせる
	/// </summary>
	inline static void GameEnd() { _isGameEnd = true; }

	/// <summary>
	/// ゲームが終わりかどうか返す
	/// </summary>
	/// <returns>終わりならtrue</returns>
	inline static BOOL GetGameEnd() { return _isGameEnd; }

private:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Init();

	/// <summary>
	/// ループ開始
	/// </summary>
	static void StartRun();

	/// <summary>
	/// ゲーム終了時に呼び出す
	/// </summary>
	static void GameFinaly();

	/// <summary>
	/// ゲームループ前に一回だけ呼び出す
	/// </summary>
	static void Start();

	/// <summary>
	/// ループ中に呼ばれる
	/// </summary>
	static void Update();

	/// <summary>
	/// ループ中に描画のために呼ばれる
	/// </summary>
	static void Render();

	/// <summary>
	/// ゲームを終わらせるかどうか
	/// </summary>
	static BOOL _isGameEnd;
};


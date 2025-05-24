#include "AllSetting.h"
#include "Game.h"
#include "Game/AudioPlayer.h"
#include "Game/LoadingScene.h"

// staticメンバの実態を定義

BOOL Game::_isGameEnd = false;

void Game::GameStart() {

	try
	{
		// ゲームフロー
		// 初期化
		Game::Init();
		// ループ開始
		Game::StartRun();
		// 終了
		Game::GameFinaly();
	}
	catch (const std::exception&)
	{
		// MessageBoxを出す
		MessageBoxA(nullptr, "ゲームを強制終了します", "", MB_OK);
	}
}

void Game::Init() {

	try
	{
		// COMの初期化に失敗したら呼び出す
		Debugger::CheckHR(CoInitializeEx(nullptr, COINIT_MULTITHREADED), "COMの初期化に失敗しました");

		// ウィンドウの作成
		DeviceManager::InitWindow();

		// デバイスの作成
		DeviceManager::InitDevice();

		// デバッグビルドではデバッグレイヤーを有効にする
#ifdef _DEBUG
		Debugger::EnableDebugLayer();
#endif

		// グラフィックスパイプラインの作成
		GraphicsManager::InitGraphics();

	}
	catch (const std::exception& e)
	{
		// ゲームを終了
		GameFinaly();
		// Messageboxで出す
		MessageBoxA(nullptr, e.what(), "ゲームの初期化中に失敗", MB_OK | MB_ICONINFORMATION);
		throw e;
	}
}

void Game::StartRun() {

	try
	{
		// 最初の一度だけ呼ばれる処理
		Game::Start();

		MSG msg = {};

		// フレーム開始時刻を記録
		auto startTime = std::chrono::high_resolution_clock::now();

		// フレーム終了時刻
		auto endTime = std::chrono::high_resolution_clock::now();

		// ループに入る前にウィンドウを出す
		ShowWindow(g_hwnd, SW_SHOW);

		// ゲームが終わるまで回す
		while (!_isGameEnd && msg.message != WM_QUIT) {

			// フレーム開始時刻を記録
			startTime = std::chrono::high_resolution_clock::now();

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// 毎フレーム呼ばれる処理
			Update();

			// シーンを変更しようとしたらそのまま次に飛ぶ(変更のタイミングでSpriteが解放されるので)
			if (SceneManager::GetChangeNow()) {
				SceneManager::GetChangeNow() = false;
				continue;
			}

			// ウィンドウへの描画全体処理
			Render();

			// フレーム終了時刻
			endTime = std::chrono::high_resolution_clock::now();

			// 経過時間を秒で取得
			std::chrono::duration<double> elapsedSec = endTime - startTime;

			// 固定フレーム時間に満たなければスリープ
			FLOAT sleepTimeSec = frameDurationS - (FLOAT)elapsedSec.count();
			if (sleepTimeSec > 0.0) {
				Sleep((DWORD)(sleepTimeSec * 1000.0f));
			}
		}
	}
	catch (const std::exception& e)
	{
		// ゲームを終了
		GameFinaly();
		// MessageBoxで出す
		MessageBoxA(nullptr, e.what(), "ゲームのループ中に失敗", MB_OK | MB_ICONINFORMATION);
		throw e;
	}
}

void Game::GameFinaly() {

	try
	{
		// 全ての解放系統を呼び出す
		SpriteObjectManager::AllDeleteSprite();
		Sprite::CacheDelete();
		SceneManager::DeleteScene();
		GraphicsManager::ReleaseGraphics();
		DeviceManager::ReleaseDevice();
		CoUninitialize();
		UnregisterClass(g_appName, GetModuleHandle(nullptr));
	}
	catch (const std::exception& e)
	{
		// MessageBoxで出す
		MessageBoxA(nullptr, e.what(), "ゲームの終了中に失敗", MB_OK | MB_ICONINFORMATION);
		throw e;
	}
}

void Game::Start() {

	try
	{
		// 最初のシーンを設定する
		SceneManager::ChangeScene(new LoadingScene());
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ゲームスタート中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void Game::Update() {

	try
	{
		// 現在のシーンのUpdateを呼び出す
		SceneManager::GetCurrentScene()->Update();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ゲームアップデート中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void Game::Render() {

	try
	{
		// 描画関係の呼び出し
		GraphicsManager::Render();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ゲーム描画中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}
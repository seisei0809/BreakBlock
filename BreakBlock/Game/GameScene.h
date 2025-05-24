#pragma once
#include "../Scene.h"
#include "BlockManager.h"
#include "BallManager.h"
#include "BarManager.h"
#include "AudioPlayer.h"
#include "SpaceKeyText.h"
#include "EscapeManager.h"

// ゲームシーン
class GameScene : public Scene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameScene();

	/// <summary>
	/// ブロック管理を取得する
	/// </summary>
	/// <returns>ブロック管理本体</returns>
	inline BlockManager* GetBlockManager() { return blockManager; }

	/// <summary>
	/// バー管理を取得する
	/// </summary>
	/// <returns>バー管理本体</returns>
	inline BarManager* GetBarManager() { return barManager; }

	/// <summary>
	/// ボール管理を取得する
	/// </summary>
	/// <returns>ボール管理本体</returns>
	inline BallManager* GetBallManager() { return ballManager; }

private:

	// BGM流す用
	AudioPlayer* bgm;

	// ブロック管理
	BlockManager* blockManager;

	// バー管理
	BarManager* barManager;

	// ボール管理
	BallManager* ballManager;
};


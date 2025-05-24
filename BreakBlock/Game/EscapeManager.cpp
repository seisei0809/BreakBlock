#include "../AllSetting.h"
#include "EscapeManager.h"
#include "../Game.h"

EscapeManager::EscapeManager() {

	try
	{
		// Spriteの定義
		escapeClick = new Sprite(imagePath);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("エスケープ管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

EscapeManager::~EscapeManager() {

}

void EscapeManager::Start(Scene*) {

	try
	{
		// とりあえずエイリアス
		Transform& tr = escapeClick->GetTransform();

		// 初期位置とサイズのセット
		tr.position = initPos;
		tr.scale = size;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("エスケープ管理Startで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void EscapeManager::Update(Scene*) {

	try
	{
		// エスケープキーを押したら直ちにゲーム終了
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {

			Game::GameEnd();
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("エスケープ管理Updateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

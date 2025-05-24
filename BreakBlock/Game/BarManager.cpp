#include "../AllSetting.h"
#include "BarManager.h"

// コンストラクタ
BarManager::BarManager() {

	try
	{
		// Spriteの定義
		bar = new Sprite(L"Bar.png");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("バー管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

// デストラクタ
BarManager::~BarManager() {

}

// 最初に呼び出される関数
void BarManager::Start(Scene*) {

	try
	{
		// とりあえず変数のエイリアス
		Transform& tr = bar->GetTransform();

		// 位置と回転の初期化
		tr.position = initPos;
		tr.scale = size;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("バー管理Startで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

// 毎フレーム呼び出される関数
void BarManager::Update(Scene*) {

	try
	{
		// とりあえず変数のエイリアス
		Transform& tr = bar->GetTransform();

		// 左と右の限界値の割り出し
		FLOAT xMin = tr.scale.x;
		FLOAT xMax = window_width - tr.scale.x;

		// Aキーか左矢印キー
		if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT) & 0x8000) {

			tr.position.x -= moveSpeed * frameDurationS;
		}
		// Dキーか右矢印キー
		if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT) & 0x8000) {

			tr.position.x += moveSpeed * frameDurationS;
		}

		// 最小値超えたら戻す
		if (tr.position.x < xMin) {
			tr.position.x = xMin;
		}
		// 最大値超えたら戻す
		else if (tr.position.x > xMax) {
			tr.position.x = xMax;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("バー管理Updateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}
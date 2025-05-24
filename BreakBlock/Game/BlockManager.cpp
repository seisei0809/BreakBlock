#include "../AllSetting.h"
#include "BlockManager.h"
#include "../Game.h"
#include "TitleScene.h"

BlockManager::BlockManager() :
	isAllNoActive(true) {

	try
	{
		// 個数分インスタンスを確保
		for (INT i = 0; i < setMat.x * setMat.y; i++) {
			blocks.push_back(new Sprite(imageName));
		}

		// レインボーの定義
		rainbow = new RainbowColor(rainbowSpeed);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ブロック管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

BlockManager::~BlockManager() {

	if (rainbow)	delete rainbow;
}

void BlockManager::Start(Scene*) {

	try
	{
		// 等間隔に並べサイズを適用
		for (size_t i = 0; i < setMat.x; i++) {
			for (size_t j = 0; j < setMat.y; j++) {
				Transform& tr = blocks[j + i * setMat.y]->GetTransform();
				tr.position.x = initPos.x + space.x * i;
				tr.position.y = initPos.y + space.y * j;
				tr.position.z = initPos.z;
				tr.scale = size;
			}
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ブロック管理Startで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void BlockManager::Update(Scene*) {

	try
	{
		// レインボー更新
		rainbow->Update();

		// 全てのブロックの色を現在の色に設定
		for (auto block : blocks) {
			block->GetColor() = rainbow->GetColor();
		}

		// 全部見てもし一つもアクティブがなかったらtrue
		isAllNoActive = true;
		for (auto block : blocks) {
			if (block->GetActive()) {
				isAllNoActive = false;
				break;
			}
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ブロック管理Updateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}
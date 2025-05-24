#include "AllSetting.h"
#include "SpriteObjectManager.h"    

// staticメンバの実体の定義

VECTOR<Sprite*> SpriteObjectManager::_sprites;

void SpriteObjectManager::AddSprite(Sprite* sprite) {

	try
	{
		// 配列のpush
		_sprites.push_back(sprite);

		// z座標が大きいほど上側(昇順)
		std::sort(_sprites.begin(), _sprites.end(), [](Sprite* a, Sprite* b) {
			return a->GetTransform().position.z < b->GetTransform().position.z;
			});
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite管理Sprite追加で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void SpriteObjectManager::DeleteSprite(Sprite* sprite) {

	try
	{
		// イテレーターで対象を探す
		auto it = std::find(_sprites.begin(), _sprites.end(), sprite);

		// 見つけたら
		if (it != _sprites.end()) {
			delete* it;			// ポインターが指すオブジェクトを削除  
			_sprites.erase(it); // イテレーターを使ってベクターから削除  
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite管理Sprite破棄で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void SpriteObjectManager::Draw() {

	try
	{
		// z座標が大きいほど上側(昇順)
		std::sort(_sprites.begin(), _sprites.end(), [](Sprite* a, Sprite* b) {
			return a->GetTransform().position.z < b->GetTransform().position.z;
			});

		// 順に描画していく
		for (auto sprite : _sprites) {

			// 描画しないなら飛ばす
			if (!sprite->GetActive())continue;
			// 描画
			sprite->Draw();
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite管理描画で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void SpriteObjectManager::AllDeleteSprite() {

	try
	{
		// 順にデリートしていく
		for (auto sprite : _sprites) {

			// Delete
			SAFE_DELETE(sprite);
		}
		// クリア
		_sprites.clear();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite管理全破棄で異常エラー:") + STR(e.what()) + STR(":"));
	}
}
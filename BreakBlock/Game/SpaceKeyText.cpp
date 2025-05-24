#include "../AllSetting.h"
#include "SpaceKeyText.h"
#include "../SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

SpaceKeyText::SpaceKeyText() :
	isUpFade(true) {

	try
	{
		// Sprite定義
		spaceClick = new Sprite(imagePath);

		// レインボー定義
		rainbow = new RainbowColor(rainbowSpeed);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("スペースキーテキストコンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

SpaceKeyText::~SpaceKeyText() {

	if (rainbow) delete rainbow;
}

void SpaceKeyText::Start(Scene*) {

	try
	{
		// とりあえずエイリアス
		Transform& tr = spaceClick->GetTransform();

		// 位置とスケーリングの初期化
		tr.position = initPos;
		tr.scale = size;
		spaceClick->GetColor().w = 0.0f;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("スペースキーテキストStartで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void SpaceKeyText::Update(Scene* scene) {

	try
	{
		// とりあえずエイリアス
		XMFLOAT4& color = spaceClick->GetColor();

		// ダイナミックキャストに成功したらタイトルシーン
		auto title = dynamic_cast<TitleScene*>(scene);
		if (title) {

			// スペースキーを押したらゲームスタート
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {

				SceneManager::ChangeScene(new GameScene());
				return;
			}

			// 最後に代入するので入れておく
			FLOAT curAlpha = color.w;

			// レインボー更新
			rainbow->Update();

			// 色のセット
			color = rainbow->GetColor();

			// 1に向かってるなら加算
			if (isUpFade) {

				curAlpha += fadeSpeed * frameDurationS;
			}
			// 0に向かってるなら減算
			else {

				curAlpha -= fadeSpeed * frameDurationS;
			}

			//  1をこえたら1に矯正して0に向かうようにする
			if (curAlpha > 1) {

				curAlpha = 1;
				isUpFade = false;
			}
			//  0をこえたら0に矯正して1に向かうようにする
			if (curAlpha < 0) {

				curAlpha = 0;
				isUpFade = true;
			}

			color.w = curAlpha;

			return;
		}
		// ダイナミックキャストに成功したらゲームシーン
		auto game = dynamic_cast<GameScene*>(scene);
		if (game) {

			// 動けるなら見えないようにする
			if (game->GetBallManager()->GetNowMove()) {

				spaceClick->SetActive(false);

				return;
			}

			// 動けないときに見えるようにする
			spaceClick->SetActive(true);

			// 最後に代入するので入れておく
			FLOAT curAlpha = color.w;

			// レインボー更新
			rainbow->Update();

			// 色のセット
			color = rainbow->GetColor();

			// 1に向かってるなら加算
			if (isUpFade) {

				curAlpha += fadeSpeed * frameDurationS;
			}
			// 0に向かってるなら減算
			else {

				curAlpha -= fadeSpeed * frameDurationS;
			}

			//  1をこえたら1に矯正して0に向かうようにする
			if (curAlpha > 1) {

				curAlpha = 1;
				isUpFade = false;
			}
			//  0をこえたら0に矯正して1に向かうようにする
			if (curAlpha < 0) {

				curAlpha = 0;
				isUpFade = true;
			}

			color.w = curAlpha;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("スペースキーテキストUpdateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}
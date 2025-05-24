#include "../AllSetting.h"
#include "LoadingManager.h"
#include "../DeviceManager.h"
#include "../Debugger.h"
#include "../SceneManager.h"
#include "TitleScene.h"

LoadingManager::LoadingManager() :
	loadIndex(),
	elapsedTime(),
	isLerp(),
	lastColor(),
	difColor() {

	try
	{
		// Spriteの定義
		loadingText = new Sprite(loadingTextName);
		completeText = new Sprite(completeTextName);
		loadingBall = new Sprite(loadingBallName);

		// フェンスオブジェクトを作成
		g_hr = DeviceManager::GetDevice()->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

		// フェンスオブジェクトの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "フェンスオブジェクトの作成に失敗しました");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ローディング管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

LoadingManager::~LoadingManager() {

	if (fence)	SAFE_RELEASE(fence);
}

void LoadingManager::Start(Scene*) {

	try
	{
		// Transformの初期化
		loadingText->GetTransform() = loadingTextTr;
		completeText->GetTransform() = completeTextTr;
		loadingBall->GetTransform() = loadingBallTr;

		// 完了文字は非アクティブ
		completeText->SetActive(false);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ローディング管理Startで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void LoadingManager::Update(Scene*) {

	try
	{

		// 配列のサイズ
		INT size = sizeof(loadingImagesName) / sizeof(loadingImagesName[0]);

		// 経過時間の加算
		elapsedTime += frameDurationS;

		// 線形移行が始まってないならボール回転
		if (!isLerp) {

			loadingBall->GetTransform().rotation += ballRotation * frameDurationS;
		}

		// GPUでの処理が終わったら毎フレーム1枚ずつロード
		if (loadIndex < size &&
			fence->GetCompletedValue() >= fenceVal) {

			new Sprite(loadingImagesName[loadIndex], fence, fenceVal);
			loadIndex++;
		}
		// もし全てのロード開始が終わって、GPUでの処理も終わって、最低delay時間を過ぎたら線形移行開始
		else if (loadIndex == size &&
			fence->GetCompletedValue() >= fenceVal && elapsedTime >= delayTime && !isLerp) {

			// ローディング系は非アクティブ
			loadingBall->SetActive(false);
			loadingText->SetActive(false);

			// 完了系はアクティブ
			completeText->SetActive(true);

			// これから線形移行開始
			isLerp = true;

			// 経過時間の初期化
			elapsedTime = 0.0f;

			// 移行前の色を保存
			for (int i = 0; i < 4; i++) {

				lastColor[i] = backColor[i];

				// どれだけ違うかも保存
				difColor[i] = completeColor[i] - lastColor[i];
			}
		}

		// 線形移行中か
		if (elapsedTime < completedTime && isLerp) {

			// 0～1で計算
			FLOAT t = elapsedTime / completedTime;

			// 色を更新
			for (int i = 0; i < 4; i++) {

				backColor[i] = lastColor[i] + difColor[i] * t;
			}
		}
		// 線形移行が終わったら背景をそのまま入れてシーンを変更
		else if (elapsedTime >= completedTime && isLerp) {

			for (int i = 0; i < 4; i++) {

				backColor[i] = completeColor[i];
			}
			SceneManager::ChangeScene(new TitleScene());
			return;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ローディング管理Updateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}
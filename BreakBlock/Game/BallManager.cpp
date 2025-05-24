#include "../AllSetting.h"
#include "BallManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "../SceneManager.h"

BallManager::BallManager() :
	speed(),
	initBlockValue(),
	delta(),
	isMove(),
	isClear(),
	isOut(),
	targetDif(),
	lastTr(),
	elapsedTime() {

	try
	{
		// Sprite定義
		ball = new Sprite(imageName);

		// 音定義
		breakSE = new AudioPlayer(audioPath);

		// レインボー定義
		rainbow = new RainbowColor(rainbowSpeed);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ボール管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

BallManager::~BallManager() {

	if (breakSE)	delete breakSE;
	if (rainbow)	delete rainbow;
}

void BallManager::Start(Scene* scene) {

	try
	{
		// とりあえずエイリアス
		Transform& tr = ball->GetTransform();
		auto main = (GameScene*)scene;

		// 初期位置とスケーリングのセット
		tr.position = initPos;
		tr.scale = size;

		// 初期速度（右上方向）
		speed = { initSpeed.x, -initSpeed.y };

		// ブロックの数を取得
		VECTOR<Sprite*> blocks = main->GetBlockManager()->GetBlocks();
		initBlockValue = blocks.size();

		// 最初は動けない
		isMove = false;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ボール管理Stratで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void BallManager::Update(Scene* scene) {

	try
	{
		// とりあえずエイリアス
		Transform& tr = ball->GetTransform();
		auto main = (GameScene*)scene;

		// もしクリアしてないときに全てのブロックを壊したら、クリアしたことにする
		if (!isClear && main->GetBlockManager()->GetAllNoActive()) {

			// もし一回でも外に出てたらシーンを即変更
			if (isOut) {

				SceneManager::ChangeScene(new TitleScene());
				return;
			}

			isClear = true;
			isMove = false;

			// ターゲットとどれだけ違うかを計算
			targetDif.position.x = targetTr.position.x - tr.position.x;
			targetDif.position.y = targetTr.position.y - tr.position.y;
			targetDif.scale.x = targetTr.scale.x - tr.scale.x;
			targetDif.scale.y = targetTr.scale.y - tr.scale.y;
			targetDif.rotation = targetTr.rotation - tr.rotation;

			// 最後の状態を保存
			lastTr = tr;
		}

		// ノーミスでクリアした時レインボーにする
		if (isClear) {

			// レインボー更新
			rainbow->Update();

			// 色更新
			ball->GetColor() = rainbow->GetColor();
		}

		// クリアしてまだ経過時間が線形時間に到達してないとき
		if (isClear && elapsedTime < lerpTime) {

			// 経過時間の加算
			elapsedTime += frameDurationS;

			// 0～1で計算
			float t = elapsedTime / lerpTime;

			// 線形補間
			tr.position.x = lastTr.position.x + targetDif.position.x * t;
			tr.position.y = lastTr.position.y + targetDif.position.y * t;
			tr.position.z = lastTr.position.z + targetDif.position.z * t;

			tr.scale.x = lastTr.scale.x + targetDif.scale.x * t;
			tr.scale.y = lastTr.scale.y + targetDif.scale.y * t;

			tr.rotation = lastTr.rotation + targetDif.rotation * t;
		}
		// クリアして経過時間が線形時間に到達してたら
		else if (isClear && elapsedTime >= lerpTime) {

			// 目標と全く同じにする
			tr = targetTr;
		}

		// クリアした時にスペース押すとシーンを変更
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && isClear) {

			SceneManager::ChangeScene(new TitleScene());
		}
		// クリアしてなくて動けないときにスペースキーで動ける
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !isMove && !isClear) {

			isMove = true;
		}

		// 動けないならリターン
		if (!isMove)return;

		// 画面端の制限
		FLOAT xMin = tr.scale.x;
		FLOAT xMax = window_width - tr.scale.x;
		FLOAT yMin = tr.scale.y;
		FLOAT yMax = window_height - tr.scale.y;

		// 移動
		tr.position.x += speed.x * frameDurationS;
		tr.position.y += speed.y * frameDurationS;
		// 回転
		tr.rotation += rotSpeed * delta * frameDurationS;

		// もし下に落ちたらやり直し
		if (tr.position.y - tr.scale.y / 2.0f > window_height) {

			// 動けなくして初期化
			isMove = false;
			tr.position = initPos;
			tr.rotation = 0.0f;
			speed.x = initSpeed.x;
			speed.y = -abs(speed.y);

			// 出たことにする
			isOut = true;

			return;
		}

		// 横方向に当たったら反転
		if (tr.position.x < xMin) {
			tr.position.x = xMin;
			speed.x = -speed.x;
		}
		else if (tr.position.x > xMax) {
			tr.position.x = xMax;
			speed.x = -speed.x;
		}

		// 縦方向に当たったら反転
		if (tr.position.y < yMin) {
			tr.position.y = yMin;
			speed.y = -speed.y;
		}

		// ブロックとの当たり判定
		VECTOR<Sprite*> blocks = main->GetBlockManager()->GetBlocks();
		for (auto block : blocks) {

			// 既に無効化されてるなら飛ばす
			if (!block->GetActive()) continue;
			// 当たったら無効化
			block->SetActive(!IsCollision(block->GetTransform(), { 1.0f, 1.0f }, false));

			// 無効化されたら一旦抜ける
			if (!block->GetActive()) {
				// 破壊音ならす
				breakSE->Play();
				break;
			}
		}

		// バーとの当たり判定
		IsCollision(main->GetBarManager()->GetBar()->GetTransform(), { 2.5f, 2.0f }, true);

		// 残りブロック数に応じてY方向の速度を調整
		int activeBlockCount = 0;
		for (auto block : blocks) {
			if (block->GetActive()) ++activeBlockCount;
		}
		speed.y = (speed.y > 0 ? 1.0f : -1.0f) *
			(initSpeed.y + addLastSpeed * ((initSpeed.y * (1.0f - ((FLOAT)activeBlockCount / (FLOAT)initBlockValue)))));
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ボール管理Updateで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

BOOL BallManager::IsCollision(Transform someTr, XMFLOAT2 mulOffset, BOOL isBar) {

	try
	{
		// とりあえずエイリアス
		Transform& tr = ball->GetTransform();

		// 自分のAABB
		XMFLOAT2 ballMin = {
			tr.position.x - tr.scale.x / 2.0f - collisionOffset.x * mulOffset.x,
			tr.position.y - tr.scale.y / 2.0f - collisionOffset.y * mulOffset.y
		};
		XMFLOAT2 ballMax = {
			tr.position.x + tr.scale.x / 2.0f + collisionOffset.x * mulOffset.x,
			tr.position.y + tr.scale.y / 2.0f + collisionOffset.y * mulOffset.y
		};

		// 相手のAABB
		XMFLOAT2 targetMin = {
			someTr.position.x - someTr.scale.x / 2.0f,
			someTr.position.y - someTr.scale.y / 2.0f
		};
		XMFLOAT2 targetMax = {
			someTr.position.x + someTr.scale.x / 2.0f,
			someTr.position.y + someTr.scale.y / 2.0f
		};

		// 当たり判定：重なっていなければfalseを返す
		if (ballMax.x < targetMin.x || ballMin.x > targetMax.x) return FALSE;
		if (ballMax.y < targetMin.y || ballMin.y > targetMax.y) return FALSE;

		// 各方向の重なり量を計算
		float overlapLeft = ballMax.x - targetMin.x;
		float overlapRight = targetMax.x - ballMin.x;
		float overlapTop = ballMax.y - targetMin.y;
		float overlapBottom = targetMax.y - ballMin.y;

		// 最小の重なり量を検出（どの方向にずれるか決める）
		float minOverlapX = std::min(overlapLeft, overlapRight);
		float minOverlapY = std::min(overlapTop, overlapBottom);

		// 立てか横のどっちの方が重なってるか
		if (minOverlapX < minOverlapY) {
			// 横方向にずれる
			if (overlapLeft < overlapRight) {
				// 右から来てないと無視
				if (speed.x <= 0.0f) return FALSE;

				tr.position.x = targetMin.x - (tr.scale.x / 2.0f) - collisionOffset.x * mulOffset.x;
			}
			else {
				// 左から来てないと無視
				if (speed.x >= 0.0f) return FALSE;

				tr.position.x = targetMax.x + (tr.scale.x / 2.0f) + collisionOffset.x * mulOffset.x;
			}

			// 反転
			speed.x = -speed.x;
		}
		else {
			// 縦方向にずれる
			// バーのみ少し違う処理
			if (isBar) {

				// バーと衝突：跳ね返り角度を左右で変化
				float relativePos = (tr.position.x - someTr.position.x) / (someTr.scale.x / 2.0f);
				relativePos = std::clamp(relativePos, -1.0f, 1.0f);
				speed.x = initSpeed.x * relativePos;

				// 回転のために入れる
				delta = relativePos;
				// 必ず上向きに跳ね返す
				speed.y = -abs(speed.y);

				// 位置補正（上に出す）
				tr.position.y = targetMin.y - (tr.scale.y / 2.0f) - collisionOffset.y * mulOffset.y;
			}
			else {
				// 上側か下側で押し出す方向を決める
				if (overlapTop < overlapBottom) {
					// 上から来てないと無視
					if (speed.y <= 0.0f) return FALSE;

					tr.position.y = targetMin.y - (tr.scale.y / 2.0f) - collisionOffset.y * mulOffset.y;
				}
				else {
					// 下から来てないと無視
					if (speed.y >= 0.0f) return FALSE;

					tr.position.y = targetMax.y + (tr.scale.y / 2.0f) + collisionOffset.y * mulOffset.y;
				}

				// 逆方向にする
				speed.y = -speed.y;
			}
		}


		return TRUE;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ボール管理当たり判定で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

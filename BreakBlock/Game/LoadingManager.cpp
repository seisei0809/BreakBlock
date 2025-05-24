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
		// Sprite�̒�`
		loadingText = new Sprite(loadingTextName);
		completeText = new Sprite(completeTextName);
		loadingBall = new Sprite(loadingBallName);

		// �t�F���X�I�u�W�F�N�g���쐬
		g_hr = DeviceManager::GetDevice()->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

		// �t�F���X�I�u�W�F�N�g�̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�t�F���X�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���[�f�B���O�Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

LoadingManager::~LoadingManager() {

	if (fence)	SAFE_RELEASE(fence);
}

void LoadingManager::Start(Scene*) {

	try
	{
		// Transform�̏�����
		loadingText->GetTransform() = loadingTextTr;
		completeText->GetTransform() = completeTextTr;
		loadingBall->GetTransform() = loadingBallTr;

		// ���������͔�A�N�e�B�u
		completeText->SetActive(false);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���[�f�B���O�Ǘ�Start�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void LoadingManager::Update(Scene*) {

	try
	{

		// �z��̃T�C�Y
		INT size = sizeof(loadingImagesName) / sizeof(loadingImagesName[0]);

		// �o�ߎ��Ԃ̉��Z
		elapsedTime += frameDurationS;

		// ���`�ڍs���n�܂��ĂȂ��Ȃ�{�[����]
		if (!isLerp) {

			loadingBall->GetTransform().rotation += ballRotation * frameDurationS;
		}

		// GPU�ł̏������I������疈�t���[��1�������[�h
		if (loadIndex < size &&
			fence->GetCompletedValue() >= fenceVal) {

			new Sprite(loadingImagesName[loadIndex], fence, fenceVal);
			loadIndex++;
		}
		// �����S�Ẵ��[�h�J�n���I����āAGPU�ł̏������I����āA�Œ�delay���Ԃ��߂�������`�ڍs�J�n
		else if (loadIndex == size &&
			fence->GetCompletedValue() >= fenceVal && elapsedTime >= delayTime && !isLerp) {

			// ���[�f�B���O�n�͔�A�N�e�B�u
			loadingBall->SetActive(false);
			loadingText->SetActive(false);

			// �����n�̓A�N�e�B�u
			completeText->SetActive(true);

			// ���ꂩ����`�ڍs�J�n
			isLerp = true;

			// �o�ߎ��Ԃ̏�����
			elapsedTime = 0.0f;

			// �ڍs�O�̐F��ۑ�
			for (int i = 0; i < 4; i++) {

				lastColor[i] = backColor[i];

				// �ǂꂾ���Ⴄ�����ۑ�
				difColor[i] = completeColor[i] - lastColor[i];
			}
		}

		// ���`�ڍs����
		if (elapsedTime < completedTime && isLerp) {

			// 0�`1�Ōv�Z
			FLOAT t = elapsedTime / completedTime;

			// �F���X�V
			for (int i = 0; i < 4; i++) {

				backColor[i] = lastColor[i] + difColor[i] * t;
			}
		}
		// ���`�ڍs���I�������w�i�����̂܂ܓ���ăV�[����ύX
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
		throw std::runtime_error(STR("���[�f�B���O�Ǘ�Update�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
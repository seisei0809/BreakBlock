#include "../AllSetting.h"
#include "SpaceKeyText.h"
#include "../SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

SpaceKeyText::SpaceKeyText() :
	isUpFade(true) {

	try
	{
		// Sprite��`
		spaceClick = new Sprite(imagePath);

		// ���C���{�[��`
		rainbow = new RainbowColor(rainbowSpeed);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�X�y�[�X�L�[�e�L�X�g�R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

SpaceKeyText::~SpaceKeyText() {

	if (rainbow) delete rainbow;
}

void SpaceKeyText::Start(Scene*) {

	try
	{
		// �Ƃ肠�����G�C���A�X
		Transform& tr = spaceClick->GetTransform();

		// �ʒu�ƃX�P�[�����O�̏�����
		tr.position = initPos;
		tr.scale = size;
		spaceClick->GetColor().w = 0.0f;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�X�y�[�X�L�[�e�L�X�gStart�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void SpaceKeyText::Update(Scene* scene) {

	try
	{
		// �Ƃ肠�����G�C���A�X
		XMFLOAT4& color = spaceClick->GetColor();

		// �_�C�i�~�b�N�L���X�g�ɐ���������^�C�g���V�[��
		auto title = dynamic_cast<TitleScene*>(scene);
		if (title) {

			// �X�y�[�X�L�[����������Q�[���X�^�[�g
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {

				SceneManager::ChangeScene(new GameScene());
				return;
			}

			// �Ō�ɑ������̂œ���Ă���
			FLOAT curAlpha = color.w;

			// ���C���{�[�X�V
			rainbow->Update();

			// �F�̃Z�b�g
			color = rainbow->GetColor();

			// 1�Ɍ������Ă�Ȃ���Z
			if (isUpFade) {

				curAlpha += fadeSpeed * frameDurationS;
			}
			// 0�Ɍ������Ă�Ȃ猸�Z
			else {

				curAlpha -= fadeSpeed * frameDurationS;
			}

			//  1����������1�ɋ�������0�Ɍ������悤�ɂ���
			if (curAlpha > 1) {

				curAlpha = 1;
				isUpFade = false;
			}
			//  0����������0�ɋ�������1�Ɍ������悤�ɂ���
			if (curAlpha < 0) {

				curAlpha = 0;
				isUpFade = true;
			}

			color.w = curAlpha;

			return;
		}
		// �_�C�i�~�b�N�L���X�g�ɐ���������Q�[���V�[��
		auto game = dynamic_cast<GameScene*>(scene);
		if (game) {

			// ������Ȃ猩���Ȃ��悤�ɂ���
			if (game->GetBallManager()->GetNowMove()) {

				spaceClick->SetActive(false);

				return;
			}

			// �����Ȃ��Ƃ��Ɍ�����悤�ɂ���
			spaceClick->SetActive(true);

			// �Ō�ɑ������̂œ���Ă���
			FLOAT curAlpha = color.w;

			// ���C���{�[�X�V
			rainbow->Update();

			// �F�̃Z�b�g
			color = rainbow->GetColor();

			// 1�Ɍ������Ă�Ȃ���Z
			if (isUpFade) {

				curAlpha += fadeSpeed * frameDurationS;
			}
			// 0�Ɍ������Ă�Ȃ猸�Z
			else {

				curAlpha -= fadeSpeed * frameDurationS;
			}

			//  1����������1�ɋ�������0�Ɍ������悤�ɂ���
			if (curAlpha > 1) {

				curAlpha = 1;
				isUpFade = false;
			}
			//  0����������0�ɋ�������1�Ɍ������悤�ɂ���
			if (curAlpha < 0) {

				curAlpha = 0;
				isUpFade = true;
			}

			color.w = curAlpha;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�X�y�[�X�L�[�e�L�X�gUpdate�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
#pragma once
#include "../Scene.h"
#include "AudioPlayer.h"
#include "SpaceKeyText.h"
#include "EscapeManager.h"

// �^�C�g���V�[��
class TitleScene : public Scene {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~TitleScene();

private:

	// BGM�����p
	AudioPlayer* bgm;

	// �^�C�g������
	Sprite* titleText;
};


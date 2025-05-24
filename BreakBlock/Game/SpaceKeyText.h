#pragma once
#include "../Object.h"
#include "RainbowColor.h"

class SpaceKeyText : public Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SpaceKeyText();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SpaceKeyText();

	/// <summary>
	/// �ŏ��ɌĂяo�����֐�
	/// </summary>
	/// <param name="">�V�[���{��</param>
	virtual void Start(Scene*);

	/// <summary>
	/// ���t���[���Ăяo�����֐�
	/// </summary>
	/// <param name="">�V�[���{��</param>
	virtual void Update(Scene*);

private:

	// �X�y�[�X�L�[�𑣂�
	Sprite* spaceClick;

	// �摜�̃p�X
	const WSTR imagePath = L"SpaceClickImage.png";

	// �ǂ����Ƀt�F�[�h���Ă邩
	BOOL isUpFade;

	// �t�F�[�h���x
	const FLOAT fadeSpeed = 5.0f;

	// �ŏ��̈ʒu
	const XMFLOAT3 initPos = { (FLOAT)window_width / 2.0f,(FLOAT)window_height / 2.0f + 120.0f,1 };

	// �傫��
	const XMFLOAT2 size = { 200.0f,200.0f };

	// ���C���{�[�p
	RainbowColor* rainbow;

	// ���C���{�[�̑���
	const FLOAT rainbowSpeed = 20.0f;
};


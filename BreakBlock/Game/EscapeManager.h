#pragma once
#include "../Object.h"
class EscapeManager : public Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EscapeManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~EscapeManager();

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

	// �G�X�P�[�v�L�[�𑣂�
	Sprite* escapeClick;

	// �摜�̃p�X
	const WSTR imagePath = L"ESCImage.png";

	// �����ʒu
	const XMFLOAT3 initPos = { 25.0f,25.0f,3.0f };

	// �T�C�Y
	const XMFLOAT2 size = { 20.0f,20.0f };
};


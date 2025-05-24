#pragma once
#include "SpriteObjectManager.h"
#include "AllSetting.h"

// �O���錾

class Scene;

// �I�u�W�F�N�g���
class Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Object() = default;

	/// <summary>
	/// �ŏ��ɌĂяo�����֐�
	/// </summary>
	/// <param name="">�V�[���{��</param>
	virtual void Start(Scene*) = 0;

	/// <summary>
	/// ���t���[���Ăяo�����֐�
	/// </summary>
	/// <param name="">�V�[���{��</param>
	virtual void Update(Scene*) = 0;
};

#pragma once
#include "../Object.h"

class BarManager : public Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BarManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BarManager();

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

	/// <summary>
	/// �o�[���擾����
	/// </summary>
	/// <returns>�o�[�̖{��</returns>
	inline Sprite* GetBar() { return bar; }

private:

	// �o�[
	Sprite* bar;

	// �o�[�̃T�C�Y
	const XMFLOAT2 size = { 70.0f,30.0f };

	// �z�u����o�[�̏����ʒu
	const XMFLOAT3 initPos = { (FLOAT)(window_width) / 2.0f,(FLOAT)window_height - 60.0f ,1.0f };

	// �����X�s�[�h
	const FLOAT moveSpeed = 1500;
};


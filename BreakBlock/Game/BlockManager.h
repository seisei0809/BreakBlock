#pragma once
#include "../Object.h"
#include "RainbowColor.h"

// �u���b�N�Ǘ�
class BlockManager : public Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BlockManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BlockManager();

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
	/// �u���b�N�z����擾����
	/// </summary>
	/// <returns>�u���b�N�{�̂̔z��</returns>
	inline VECTOR<Sprite*> GetBlocks() { return blocks; }

	/// <summary>
	/// �S�Ă���A�N�e�B�u�ɂȂ������ǂ������擾����
	/// </summary>
	/// <returns>�S�Ĕ�A�N�e�B�u�Ȃ�true</returns>
	inline BOOL GetAllNoActive() const { return isAllNoActive; }
	
private:

	// �z�u����u���b�N�̔z��
	VECTOR<Sprite*> blocks;

	// �摜�̖��O
	const WSTR imageName = L"BallAndBlock.png";

	// �u���b�N�̃T�C�Y
	const XMFLOAT2 size = { 60.0f,10.0f };

	// �z�u����u���b�N�̏����ʒu
	const XMFLOAT3 initPos = { 120.0f,20.0f,1.0f };

	// �z�u����Ԋu
	const XMFLOAT2 space = { 140.0f,30.0f };

	// �s�Ɨ񉽌u����
	const XMINT2 setMat = { 5,10 };

	// �u���b�N�����C���{�[�ω�������
	RainbowColor* rainbow;

	// ���C���{�[�ω��̑��x
	const FLOAT rainbowSpeed = 10.0f;

	// �S�Ă���A�N�e�B�u�ɂȂ������ǂ���
	BOOL isAllNoActive;
};


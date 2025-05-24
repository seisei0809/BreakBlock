#pragma once
#include "../Object.h"
#include "AudioPlayer.h"
#include "RainbowColor.h"

class BallManager : public Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BallManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BallManager();

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
	/// �������Ă邩
	/// </summary>
	/// <returns>�����Ă�Ȃ�true</returns>
	inline BOOL GetNowMove() const { return isMove; }

private:

	/// <summary>
	/// ��ʓ��̉����ɏՓ˂����̂����m
	/// </summary>
	/// <param name="someTr">����Transform</param>
	/// <param name="mulOffset">�I�t�Z�b�g�͉��{�����邩</param>
	/// <param name="isBar">�o�[���ǂ���</param>
	/// <returns>�Փ˂������ǂ���</returns>
	BOOL IsCollision(Transform, XMFLOAT2, BOOL);

	// �{�[��
	Sprite* ball;

	// �摜�̖��O
	const WSTR imageName = L"BallAndBlock.png";

	// �{�[���̃T�C�Y
	const XMFLOAT2 size = { 20.0f,20.0f };

	// �z�u����{�[���̏����ʒu
	const XMFLOAT3 initPos = { (FLOAT)(window_width) / 2.0f,(FLOAT)(window_height)-130.0f ,3.0f };

	// ���x
	XMFLOAT2 speed;

	// �����̑��x
	const XMFLOAT2 initSpeed = { 600.0f,600.0f };

	// �ŏI�I�ɉ��{�̑��x�𑫂���
	const FLOAT addLastSpeed = 1.5f;

	// ��]���x
	const FLOAT rotSpeed = 30.0f;

	// 0�`1���󂯎��
	FLOAT delta;

	// �Փˈʒu���I�t�Z�b�g
	const XMFLOAT2 collisionOffset = { 25.0f,5.0f };

	// �ŏ��̃u���b�N�̐�
	UINT64 initBlockValue;

	// �����邩�ǂ���
	BOOL isMove;

	// �Q�[���N���A�������ǂ���
	BOOL isClear;

	// ���ł��A�E�g������
	BOOL isOut;

	// �Q�[���I�����Ɉ����A�E�g���Ȃ��Ȃ炱�̖ڕW�l�ɂ��邽��
	const Transform targetTr = { {window_width / 2.0f,window_height / 2.0f - 100.0f,2.0f},0.0f,{200.0f,200.0f} };

	// �ڕW�l�Ƃǂꂾ���������邩
	Transform targetDif;

	// �N���A���Ō��Transform���
	Transform lastTr;

	// ���`�I�ɉ��b�ŖڕW��Transform�܂Ŏ����Ă�����
	const FLOAT lerpTime = 1.0f;

	// �o�ߎ���
	FLOAT elapsedTime;

	// �����Ƃ��̉�
	AudioPlayer* breakSE;

	// ���̃p�X
	const WSTR audioPath = L"BreakSE.wav";

	// �N���A���Ƀ��C���{�[�ɂ���
	RainbowColor* rainbow;

	// ���C���{�[�̑���
	const FLOAT rainbowSpeed = 20.0f;
};


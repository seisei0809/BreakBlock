#pragma once
#include "../Object.h"

// ���[�f�B���O�Ǘ�
class LoadingManager : public Object {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LoadingManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~LoadingManager();

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

	// ���[�f�B���O�ɏo������
	Sprite* loadingText;

	// ���[�f�B���O�����̖��O
	const WSTR loadingTextName = L"LoadingImage.png";

	// ���[�f�B���O������Transform
	const Transform loadingTextTr =
	{ {(FLOAT)window_width - 150.0f,(FLOAT)window_height - 30.0f,2.0f},0.0f,{150.0f,150.0f} };

	// �����������ɏo������
	Sprite* completeText;

	// ���������̖��O
	const WSTR completeTextName = L"CompleteImage.png";

	// ����������Transform
	const Transform completeTextTr =
	{ {(FLOAT)window_width - 150.0f,(FLOAT)window_height - 30.0f,1.0f},0.0f,{150.0f,150.0f} };

	// ���[�f�B���O���ɏo���{�[��
	Sprite* loadingBall;

	// ���[�f�B���O�{�[���̖��O
	const WSTR loadingBallName = L"BallAndBlock.png";

	// �{�[���̉�]�X�s�[�h
	const FLOAT ballRotation = 10.0f;

	// ���[�f�B���O�{�[����Transform
	const Transform loadingBallTr =
	{ {(FLOAT)window_width - 20.0f,(FLOAT)window_height - 30.0f,1.0f},0.0f,{20.0f,20.0f} };

	// ���[�f�B���O����摜�̖��O
	const WSTR loadingImagesName[4] =
	{ L"Bar.png",L"ESCImage.png",L"SpaceClickImage.png",L"TitleImage.png" };

	// ���[�f�B���O��p�t�F���X
	ID3D12Fence* fence;

	// ��p�̃t�F���X�̊�����
	UINT64 fenceVal;

	// �����܂ōŒች�b��delay�ɂ��邩(������h�~)
	const FLOAT delayTime = 1.0f;

	// �o�ߎ���
	FLOAT elapsedTime;

	// �ڍs�O�̔w�i�F
	FLOAT lastColor[4];

	// ������̔w�i�F
	const FLOAT completeColor[4] = { 1.0f,1.0f,0.0f,1.0f };

	// �ڍs�O�Ɗ����オ�ǂꂾ���Ⴄ��
	FLOAT difColor[4];

	// �����㉽�b�őJ�ڂ��邩
	const FLOAT completedTime = 1.0f;

	// ���`�ڍs����
	BOOL isLerp;

	// �����ڂ̃��[�h��
	INT loadIndex;
};


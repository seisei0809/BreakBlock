#pragma once
#include "AllSetting.h"
#include "Debugger.h"
#include "DeviceManager.h"
#include "GraphicsManager.h"
#include "SpriteObjectManager.h"
#include "SceneManager.h"

// �Q�[���̍���
class Game {
public:

	/// <summary>
	/// �Q�[�����J�n����
	/// </summary>
	static void GameStart();

	/// <summary>
	/// �Q�[�����I��点��
	/// </summary>
	inline static void GameEnd() { _isGameEnd = true; }

	/// <summary>
	/// �Q�[�����I��肩�ǂ����Ԃ�
	/// </summary>
	/// <returns>�I���Ȃ�true</returns>
	inline static BOOL GetGameEnd() { return _isGameEnd; }

private:

	/// <summary>
	/// ������
	/// </summary>
	static void Init();

	/// <summary>
	/// ���[�v�J�n
	/// </summary>
	static void StartRun();

	/// <summary>
	/// �Q�[���I�����ɌĂяo��
	/// </summary>
	static void GameFinaly();

	/// <summary>
	/// �Q�[�����[�v�O�Ɉ�񂾂��Ăяo��
	/// </summary>
	static void Start();

	/// <summary>
	/// ���[�v���ɌĂ΂��
	/// </summary>
	static void Update();

	/// <summary>
	/// ���[�v���ɕ`��̂��߂ɌĂ΂��
	/// </summary>
	static void Render();

	/// <summary>
	/// �Q�[�����I��点�邩�ǂ���
	/// </summary>
	static BOOL _isGameEnd;
};


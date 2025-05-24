#pragma once
#include "../Scene.h"
#include "BlockManager.h"
#include "BallManager.h"
#include "BarManager.h"
#include "AudioPlayer.h"
#include "SpaceKeyText.h"
#include "EscapeManager.h"

// �Q�[���V�[��
class GameScene : public Scene {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GameScene();

	/// <summary>
	/// �u���b�N�Ǘ����擾����
	/// </summary>
	/// <returns>�u���b�N�Ǘ��{��</returns>
	inline BlockManager* GetBlockManager() { return blockManager; }

	/// <summary>
	/// �o�[�Ǘ����擾����
	/// </summary>
	/// <returns>�o�[�Ǘ��{��</returns>
	inline BarManager* GetBarManager() { return barManager; }

	/// <summary>
	/// �{�[���Ǘ����擾����
	/// </summary>
	/// <returns>�{�[���Ǘ��{��</returns>
	inline BallManager* GetBallManager() { return ballManager; }

private:

	// BGM�����p
	AudioPlayer* bgm;

	// �u���b�N�Ǘ�
	BlockManager* blockManager;

	// �o�[�Ǘ�
	BarManager* barManager;

	// �{�[���Ǘ�
	BallManager* ballManager;
};


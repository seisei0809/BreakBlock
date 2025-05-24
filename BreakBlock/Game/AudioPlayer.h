#pragma once
#include "../AllSetting.h"

// ���Đ�
class AudioPlayer {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="path">�p�X��</param>
	AudioPlayer(WSTR);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~AudioPlayer();

	/// <summary>
	/// ����������wave�t�@�C�������[�h����
	/// </summary>
	/// <param name="data">�f�[�^</param>
	void LoadWavFromMemory(const std::vector<UINT8>);

	/// <summary>
	/// �Đ�����
	/// </summary>
	/// <param name="loop">���[�v���邩�ǂ���</param>
	void Play(bool = false);

	/// <summary>
	/// �d�����ė���Ă���̂◬���ς݂̂��̂��܂Ƃ߂��Y��ɂ���
	/// </summary>
	void ClearAll();

private:

	// XAudion�̃f�o�C�X(�{��)
	IXAudio2* xaudio2;

	// �}�X�^�[�{�C�X(�X�s�[�J�[�ƂȂ���ŏI�n�_)
	IXAudio2MasteringVoice* masterVoice;

	// �Đ��̂��߂ɕK�v�ȉ��f�[�^
	std::vector<UINT8> audioData;

	// wave�t�@�C���̃t�H�[�}�b�g
	WAVEFORMATEX waveFormat;

	// ���o�b�t�@�̊Ǘ��z��
	std::vector<IXAudio2SourceVoice*> activeVoices;
};
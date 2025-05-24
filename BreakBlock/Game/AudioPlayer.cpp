#include "../AllSetting.h"
#include "AudioPlayer.h"
#include "../Debugger.h"
#include "../AssetManager.h"

AudioPlayer::AudioPlayer(WSTR path)
	:
	xaudio2(nullptr),
	masterVoice(nullptr),
	waveFormat(WAVEFORMATEX()) {

	try
	{
		// XAudio2�f�o�C�X�̍쐬
		g_hr = XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
		Debugger::CheckHR(g_hr, "XAudio2�̏������Ɏ��s");

		// �}�X�^�[�{�C�X�̍쐬
		g_hr = xaudio2->CreateMasteringVoice(&masterVoice);
		Debugger::CheckHR(g_hr, "�}�X�^�[�{�C�X�̍쐬�Ɏ��s");

		// �}�X�^�[�{�C�X�̉��ʂ��ő�ɐݒ�
		g_hr = masterVoice->SetVolume(1.0f);
		Debugger::CheckHR(g_hr, "�}�X�^�[�{�C�X�̉��ʐݒ�Ɏ��s");

		// �p�b�N�t�@�C������WAV�����[�h
		auto packedFiles = AssetManager::LoadPack();
		for (auto& file : packedFiles) {

			// �w�肳�ꂽ�p�X�ƈ�v����t�@�C����T��
			if (file.name == path) {

				// ��v������WAV�f�[�^������������ǂݍ���
				LoadWavFromMemory(file.data);
			}
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

AudioPlayer::~AudioPlayer() {

	ClearAll();

	if (masterVoice) {

		// �}�X�^�[�{�C�X�폜
		masterVoice->DestroyVoice();
		masterVoice = nullptr;
	}

	if (xaudio2) SAFE_RELEASE(xaudio2);
}

void AudioPlayer::LoadWavFromMemory(const std::vector<UINT8> data) {

	try
	{
		// wave�t�@�C���͍Œ�44�o�C�g�ȏ゠��
		if (data.size() < 44) {
			throw std::runtime_error("WAV�f�[�^������������ (44byte����)");
		}

		const CHAR* ptr = (CHAR*)(data.data()); // �擪�|�C���^�擾
		const CHAR* end = ptr + data.size();    // �I�[�|�C���^

		// RIFF/WAVE�w�b�_�̊m�F
		if (std::memcmp(ptr, "RIFF", 4) != 0 || std::memcmp(ptr + 8, "WAVE", 4) != 0) {
			throw std::runtime_error("������WAV�w�b�_");
		}

		// �`�����N�̐擪�i�w�b�_���X�L�b�v�j
		const CHAR* p = ptr + 12;

		bool fmtFound = false;
		bool dataFound = false;

		// �`�����N�����ɑ���
		while (p + 8 <= end) {

			const CHAR* chunkId = p;                // �`�����N���i"fmt "�Ȃǁj
			UINT32 chunkSize = *(UINT32*)(p + 4);   // �`�����N�T�C�Y
			const CHAR* chunkData = p + 8;          // �`�����N�̃f�[�^��

			// �`�����N�T�C�Y�ُ�`�F�b�N
			if (chunkData + chunkSize > end) {
				throw std::runtime_error("�`�����N�T�C�Y�ُ�");
			}

			// fmt�`�����N�̏ꍇ�i�����t�H�[�}�b�g�j
			if (std::memcmp(chunkId, "fmt ", 4) == 0) {

				// �t�H�[�}�b�gID�擾
				USHORT formatTag = *(USHORT*)(chunkData);

				if (formatTag != WAVE_FORMAT_PCM) {
					throw std::runtime_error("��PCM�`����WAV�B�Ή����Ă܂���");
				}

				// 16�o�C�g�̏ꍇ�W��PCM�Ȃ̂Ŋg���t�H�[�}�b�g�ł͂Ȃ�
				if (chunkSize == 16) {

					// 16�o�C�g�̂݃t�H�[�}�b�g�\���̂ɃR�s�[
					std::memcpy(&waveFormat, chunkData, 16);
					// �g���T�C�Y�Ȃ��ɐݒ�
					waveFormat.cbSize = 0;
				}
				else if (chunkSize >= sizeof(WAVEFORMATEX)) {

					// �T�C�Y���t�H�[�}�b�g�\���̂ɃR�s�[
					std::memcpy(&waveFormat, chunkData, sizeof(WAVEFORMATEX));
				}
				else {
					throw std::runtime_error("fmt�`�����N��16�o�C�g�ȏ�K�v�ł�");
				}

				// ���������Ƃɂ���
				fmtFound = true;
			}
			// data�`�����N�̏ꍇ�i���ۂ̉����f�[�^�j
			else if (std::memcmp(chunkId, "data", 4) == 0) {
				// �f�[�^�T�C�Y�̒���
				audioData.resize(chunkSize);
				// �T�C�Y���f�[�^���R�s�[
				std::memcpy(audioData.data(), chunkData, chunkSize);

				// ���������Ƃɂ���
				dataFound = true;
			}

			// ���̃`�����N��
			p += 8 + chunkSize;
		}

		if (!fmtFound) {
			throw std::runtime_error("fmt�`�����N��������Ȃ�");
		}
		if (!dataFound) {
			throw std::runtime_error("data�`�����N��������Ȃ�");
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���Ǘ����[�h�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void AudioPlayer::Play(bool loop) {

	try
	{
		// ����������Ă��Ȃ���΍Đ����Ȃ�
		if (!xaudio2 || audioData.empty()) return;

		// ���o�b�t�@�̊m��
		IXAudio2SourceVoice* newVoice = nullptr;
		g_hr = xaudio2->CreateSourceVoice(&newVoice, &waveFormat);
		Debugger::CheckHR(g_hr, "SourceVoice�̍쐬�Ɏ��s");

		XAUDIO2_BUFFER buffer = {};
		buffer.AudioBytes = (UINT32)(audioData.size());         // �����f�[�^�T�C�Y
		buffer.pAudioData = audioData.data();                   // �����f�[�^�|�C���^
		buffer.Flags = loop ? 0 : XAUDIO2_END_OF_STREAM;        // ���[�v�Đ��ݒ�
		buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;    // ���[�v�񐔐ݒ�(���[�v�Ȃ疳���A�Ⴄ�Ȃ�0)

		// �o�b�t�@�𑗐M
		g_hr = newVoice->SubmitSourceBuffer(&buffer);
		if (FAILED(g_hr)) {
			newVoice->DestroyVoice();
			throw std::runtime_error("SourceBuffer�̑��M�Ɏ��s");
		}

		// �Đ��J�n
		g_hr = newVoice->Start(0);
		if (FAILED(g_hr)) {
			newVoice->DestroyVoice();
			throw std::runtime_error("�����Đ��J�n�Ɏ��s");
		}

		// �m�ۂ������o�b�t�@�����X�g�ɕۑ�
		activeVoices.push_back(newVoice);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���Ǘ��Đ��ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void AudioPlayer::ClearAll() {

	try
	{
		// �S�Ď~�߂đS�Ĕj��
		for (auto& voice : activeVoices) {
			if (voice) {
				voice->Stop(0);
				voice->DestroyVoice();
			}
		}
		activeVoices.clear();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("���Ǘ��N���[���ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

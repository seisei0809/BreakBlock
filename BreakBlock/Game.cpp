#include "AllSetting.h"
#include "Game.h"
#include "Game/AudioPlayer.h"
#include "Game/LoadingScene.h"

// static�����o�̎��Ԃ��`

BOOL Game::_isGameEnd = false;

void Game::GameStart() {

	try
	{
		// �Q�[���t���[
		// ������
		Game::Init();
		// ���[�v�J�n
		Game::StartRun();
		// �I��
		Game::GameFinaly();
	}
	catch (const std::exception&)
	{
		// MessageBox���o��
		MessageBoxA(nullptr, "�Q�[���������I�����܂�", "", MB_OK);
	}
}

void Game::Init() {

	try
	{
		// COM�̏������Ɏ��s������Ăяo��
		Debugger::CheckHR(CoInitializeEx(nullptr, COINIT_MULTITHREADED), "COM�̏������Ɏ��s���܂���");

		// �E�B���h�E�̍쐬
		DeviceManager::InitWindow();

		// �f�o�C�X�̍쐬
		DeviceManager::InitDevice();

		// �f�o�b�O�r���h�ł̓f�o�b�O���C���[��L���ɂ���
#ifdef _DEBUG
		Debugger::EnableDebugLayer();
#endif

		// �O���t�B�b�N�X�p�C�v���C���̍쐬
		GraphicsManager::InitGraphics();

	}
	catch (const std::exception& e)
	{
		// �Q�[�����I��
		GameFinaly();
		// Messagebox�ŏo��
		MessageBoxA(nullptr, e.what(), "�Q�[���̏��������Ɏ��s", MB_OK | MB_ICONINFORMATION);
		throw e;
	}
}

void Game::StartRun() {

	try
	{
		// �ŏ��̈�x�����Ă΂�鏈��
		Game::Start();

		MSG msg = {};

		// �t���[���J�n�������L�^
		auto startTime = std::chrono::high_resolution_clock::now();

		// �t���[���I������
		auto endTime = std::chrono::high_resolution_clock::now();

		// ���[�v�ɓ���O�ɃE�B���h�E���o��
		ShowWindow(g_hwnd, SW_SHOW);

		// �Q�[�����I���܂ŉ�
		while (!_isGameEnd && msg.message != WM_QUIT) {

			// �t���[���J�n�������L�^
			startTime = std::chrono::high_resolution_clock::now();

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// ���t���[���Ă΂�鏈��
			Update();

			// �V�[����ύX���悤�Ƃ����炻�̂܂܎��ɔ��(�ύX�̃^�C�~���O��Sprite����������̂�)
			if (SceneManager::GetChangeNow()) {
				SceneManager::GetChangeNow() = false;
				continue;
			}

			// �E�B���h�E�ւ̕`��S�̏���
			Render();

			// �t���[���I������
			endTime = std::chrono::high_resolution_clock::now();

			// �o�ߎ��Ԃ�b�Ŏ擾
			std::chrono::duration<double> elapsedSec = endTime - startTime;

			// �Œ�t���[�����Ԃɖ����Ȃ���΃X���[�v
			FLOAT sleepTimeSec = frameDurationS - (FLOAT)elapsedSec.count();
			if (sleepTimeSec > 0.0) {
				Sleep((DWORD)(sleepTimeSec * 1000.0f));
			}
		}
	}
	catch (const std::exception& e)
	{
		// �Q�[�����I��
		GameFinaly();
		// MessageBox�ŏo��
		MessageBoxA(nullptr, e.what(), "�Q�[���̃��[�v���Ɏ��s", MB_OK | MB_ICONINFORMATION);
		throw e;
	}
}

void Game::GameFinaly() {

	try
	{
		// �S�Ẳ���n�����Ăяo��
		SpriteObjectManager::AllDeleteSprite();
		Sprite::CacheDelete();
		SceneManager::DeleteScene();
		GraphicsManager::ReleaseGraphics();
		DeviceManager::ReleaseDevice();
		CoUninitialize();
		UnregisterClass(g_appName, GetModuleHandle(nullptr));
	}
	catch (const std::exception& e)
	{
		// MessageBox�ŏo��
		MessageBoxA(nullptr, e.what(), "�Q�[���̏I�����Ɏ��s", MB_OK | MB_ICONINFORMATION);
		throw e;
	}
}

void Game::Start() {

	try
	{
		// �ŏ��̃V�[����ݒ肷��
		SceneManager::ChangeScene(new LoadingScene());
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�Q�[���X�^�[�g���Ɉُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void Game::Update() {

	try
	{
		// ���݂̃V�[����Update���Ăяo��
		SceneManager::GetCurrentScene()->Update();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�Q�[���A�b�v�f�[�g���Ɉُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void Game::Render() {

	try
	{
		// �`��֌W�̌Ăяo��
		GraphicsManager::Render();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�Q�[���`�撆�Ɉُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
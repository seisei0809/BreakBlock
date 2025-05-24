#include "AllSetting.h"
#include "SceneManager.h"

// static�����o�̎��̂��`

Scene* SceneManager::_currentScene = nullptr;
BOOL SceneManager::_isChangeNow = false;

void SceneManager::ChangeScene(Scene* newScene) {

	try
	{
		// ���݂̃V�[��������Ȃ�
		if (_currentScene) {

			// �j��
			delete _currentScene;
		}
		// �V�[����ύX����Ƃ���
		_isChangeNow = true;

		// �V�����V�[���̐ݒ�
		_currentScene = newScene;

		// Start���Ƃ肠�����Ă�
		_currentScene->Start();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�V�[���Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void SceneManager::DeleteScene() {

	try
	{
		if (_currentScene)   delete _currentScene;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�V�[���Ǘ��V�[���j���ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

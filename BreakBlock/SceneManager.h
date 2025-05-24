#pragma once
#include "Scene.h"

// �V�[���Ǘ�
class SceneManager {
public:

	/// <summary>
	/// �V�[����ύX����
	/// </summary>
	/// <param name="newScene">�V�����V�[��</param>
	static void ChangeScene(Scene*);

	/// <summary>
	/// ���݂̃V�[�����擾����
	/// </summary>
	/// <returns>���݂̃V�[���{��</returns>
	inline static Scene* GetCurrentScene() { return _currentScene; }

	/// <summary>
	/// �V�[����j������
	/// </summary>
	static void DeleteScene();

	/// <summary>
	/// ���݃V�[����ύX���悤�Ƃ��Ă��邩
	/// </summary>
	/// <returns>���悤�Ƃ��Ă�Ȃ�true</returns>
	inline static BOOL& GetChangeNow() { return _isChangeNow; }

private:

	// ���݂̃V�[��
	static Scene* _currentScene;

	// ���݃V�[����ύX���悤�Ƃ��Ă��邩
	static BOOL _isChangeNow;
};

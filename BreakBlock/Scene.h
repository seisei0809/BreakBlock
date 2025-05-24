#pragma once
#include "AllSetting.h"
#include "Object.h"

// �V�[�����
class Scene {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Scene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Scene();

	/// <summary>
	/// Object��Start�֐����Ăяo��
	/// </summary>
	void Start();

	/// <summary>
	/// Object��Update�֐����Ăяo��
	/// </summary>
	void Update();

protected:

	// Object��ǉ�����(template�̓R���p�C�����ɃC���X�^���X������邩���`�̓w�b�_��)  
	template <typename T> T* AddObject() {

		// Object���ȊO�̃N���X�Ȃ�R���p�C�����ɃG���[���o��
		static_assert(std::is_base_of<Object, T>::value, "T�N���X�͕K��Object���p�����Ă��邱��");

		// �w�肳�ꂽ�^�̃C���X�^���X���쐬
		T* obj = new T();

		// objects �ɒǉ�
		objects.push_back(obj);

		return obj;
	}

	// Object�|�C���^���i�[
	VECTOR<Object*> objects;
};

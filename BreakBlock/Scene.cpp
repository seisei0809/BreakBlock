#include "AllSetting.h"
#include "Scene.h"
#include "SceneManager.h"

Scene::Scene() {

	try
	{
		// �Ƃ肠�����S������
		SpriteObjectManager::AllDeleteSprite();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�V�[���R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

Scene::~Scene() {

	// �S�Ă�Object�����
	for (auto obj : objects) {

		delete obj;
	}
	// �N���A
	objects.clear();
}

void Scene::Start() {

	try
	{
		// �S�Ă�Start���\�b�h���Ăяo��
		for (auto obj : objects) {

			obj->Start(this);
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�V�[��Start�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void Scene::Update() {

	try
	{
		// �S�Ă�Update���\�b�h���Ăяo��
		for (auto obj : objects) {

			// �V�[���̕ύX�����m���Ă܂����C�Ȃ獡����������
			if (SceneManager::GetChangeNow())break;

			obj->Update(this);
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�V�[��Update�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
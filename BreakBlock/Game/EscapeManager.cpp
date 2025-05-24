#include "../AllSetting.h"
#include "EscapeManager.h"
#include "../Game.h"

EscapeManager::EscapeManager() {

	try
	{
		// Sprite�̒�`
		escapeClick = new Sprite(imagePath);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�G�X�P�[�v�Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

EscapeManager::~EscapeManager() {

}

void EscapeManager::Start(Scene*) {

	try
	{
		// �Ƃ肠�����G�C���A�X
		Transform& tr = escapeClick->GetTransform();

		// �����ʒu�ƃT�C�Y�̃Z�b�g
		tr.position = initPos;
		tr.scale = size;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�G�X�P�[�v�Ǘ�Start�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void EscapeManager::Update(Scene*) {

	try
	{
		// �G�X�P�[�v�L�[���������璼���ɃQ�[���I��
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {

			Game::GameEnd();
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�G�X�P�[�v�Ǘ�Update�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

#include "../AllSetting.h"
#include "BarManager.h"

// �R���X�g���N�^
BarManager::BarManager() {

	try
	{
		// Sprite�̒�`
		bar = new Sprite(L"Bar.png");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�o�[�Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

// �f�X�g���N�^
BarManager::~BarManager() {

}

// �ŏ��ɌĂяo�����֐�
void BarManager::Start(Scene*) {

	try
	{
		// �Ƃ肠�����ϐ��̃G�C���A�X
		Transform& tr = bar->GetTransform();

		// �ʒu�Ɖ�]�̏�����
		tr.position = initPos;
		tr.scale = size;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�o�[�Ǘ�Start�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

// ���t���[���Ăяo�����֐�
void BarManager::Update(Scene*) {

	try
	{
		// �Ƃ肠�����ϐ��̃G�C���A�X
		Transform& tr = bar->GetTransform();

		// ���ƉE�̌��E�l�̊���o��
		FLOAT xMin = tr.scale.x;
		FLOAT xMax = window_width - tr.scale.x;

		// A�L�[�������L�[
		if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT) & 0x8000) {

			tr.position.x -= moveSpeed * frameDurationS;
		}
		// D�L�[���E���L�[
		if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT) & 0x8000) {

			tr.position.x += moveSpeed * frameDurationS;
		}

		// �ŏ��l��������߂�
		if (tr.position.x < xMin) {
			tr.position.x = xMin;
		}
		// �ő�l��������߂�
		else if (tr.position.x > xMax) {
			tr.position.x = xMax;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�o�[�Ǘ�Update�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
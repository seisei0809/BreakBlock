#include "../AllSetting.h"
#include "BlockManager.h"
#include "../Game.h"
#include "TitleScene.h"

BlockManager::BlockManager() :
	isAllNoActive(true) {

	try
	{
		// �����C���X�^���X���m��
		for (INT i = 0; i < setMat.x * setMat.y; i++) {
			blocks.push_back(new Sprite(imageName));
		}

		// ���C���{�[�̒�`
		rainbow = new RainbowColor(rainbowSpeed);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�u���b�N�Ǘ��R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

BlockManager::~BlockManager() {

	if (rainbow)	delete rainbow;
}

void BlockManager::Start(Scene*) {

	try
	{
		// ���Ԋu�ɕ��׃T�C�Y��K�p
		for (size_t i = 0; i < setMat.x; i++) {
			for (size_t j = 0; j < setMat.y; j++) {
				Transform& tr = blocks[j + i * setMat.y]->GetTransform();
				tr.position.x = initPos.x + space.x * i;
				tr.position.y = initPos.y + space.y * j;
				tr.position.z = initPos.z;
				tr.scale = size;
			}
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�u���b�N�Ǘ�Start�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void BlockManager::Update(Scene*) {

	try
	{
		// ���C���{�[�X�V
		rainbow->Update();

		// �S�Ẵu���b�N�̐F�����݂̐F�ɐݒ�
		for (auto block : blocks) {
			block->GetColor() = rainbow->GetColor();
		}

		// �S�����Ă�������A�N�e�B�u���Ȃ�������true
		isAllNoActive = true;
		for (auto block : blocks) {
			if (block->GetActive()) {
				isAllNoActive = false;
				break;
			}
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�u���b�N�Ǘ�Update�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
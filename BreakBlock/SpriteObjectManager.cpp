#include "AllSetting.h"
#include "SpriteObjectManager.h"    

// static�����o�̎��̂̒�`

VECTOR<Sprite*> SpriteObjectManager::_sprites;

void SpriteObjectManager::AddSprite(Sprite* sprite) {

	try
	{
		// �z���push
		_sprites.push_back(sprite);

		// z���W���傫���قǏ㑤(����)
		std::sort(_sprites.begin(), _sprites.end(), [](Sprite* a, Sprite* b) {
			return a->GetTransform().position.z < b->GetTransform().position.z;
			});
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�Ǘ�Sprite�ǉ��ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void SpriteObjectManager::DeleteSprite(Sprite* sprite) {

	try
	{
		// �C�e���[�^�[�őΏۂ�T��
		auto it = std::find(_sprites.begin(), _sprites.end(), sprite);

		// ��������
		if (it != _sprites.end()) {
			delete* it;			// �|�C���^�[���w���I�u�W�F�N�g���폜  
			_sprites.erase(it); // �C�e���[�^�[���g���ăx�N�^�[����폜  
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�Ǘ�Sprite�j���ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void SpriteObjectManager::Draw() {

	try
	{
		// z���W���傫���قǏ㑤(����)
		std::sort(_sprites.begin(), _sprites.end(), [](Sprite* a, Sprite* b) {
			return a->GetTransform().position.z < b->GetTransform().position.z;
			});

		// ���ɕ`�悵�Ă���
		for (auto sprite : _sprites) {

			// �`�悵�Ȃ��Ȃ��΂�
			if (!sprite->GetActive())continue;
			// �`��
			sprite->Draw();
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�Ǘ��`��ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void SpriteObjectManager::AllDeleteSprite() {

	try
	{
		// ���Ƀf���[�g���Ă���
		for (auto sprite : _sprites) {

			// Delete
			SAFE_DELETE(sprite);
		}
		// �N���A
		_sprites.clear();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�Ǘ��S�j���ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
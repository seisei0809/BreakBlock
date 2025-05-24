#include "AllSetting.h"
#include "AssetManager.h"

// static�����o�̎��̂��`����

WSTR AssetManager::_packPath = L"SeiseiPack.asset";

VECTOR<PackedFile> AssetManager::LoadPack() {

	try
	{
		VECTOR<PackedFile> files;  // ���������t�@�C�����i�[����x�N�^�[

		// �o�C�i�����[�h�œ��̓t�@�C�����J��
		std::ifstream inFile(_packPath, std::ios::binary);

		if (!inFile) return files;  // �J���Ȃ���΋�̂܂܏I��

		while (true) {

			UINT nameLength = 0;

			// �t�@�C�����̒�����ǂݍ���
			inFile.read((char*)&nameLength, sizeof(nameLength));

			if (inFile.eof() || inFile.fail()) break;  // �ǂݍ��ݎ��s��I�[�Ȃ�I��

			WSTR fileName;
			fileName.resize(nameLength);  // �t�@�C�����̒������o�b�t�@���m��

			// �t�@�C�����iwchar_t�z��j��ǂݍ���
			inFile.read((char*)&fileName[0], nameLength * sizeof(wchar_t));
			if (inFile.fail()) break;  // ���s������I��

			UINT dataSize = 0;

			// �t�@�C���f�[�^�̃T�C�Y�i�o�C�g���j��ǂݍ��ށi4�o�C�g�j
			inFile.read((char*)&dataSize, sizeof(dataSize));
			if (inFile.fail()) break;

			// �f�[�^�{�̂�ǂݍ��ނ��߂̃o�b�t�@���m��
			VECTOR<UINT8> data(dataSize);

			if (dataSize > 0) {
				// ���ۂ̃f�[�^��ǂݍ���
				inFile.read((char*)&data[0], dataSize);
				if (inFile.fail()) break;
			}

			// �ǂݍ��񂾃t�@�C�����ƃf�[�^���\���̂ɂ܂Ƃ߂Ēǉ�
			files.push_back({ std::move(fileName), std::move(data) });
		}

		return files;  // �S�t�@�C�����i�[�����x�N�^�[��Ԃ�
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�A�Z�b�g�t�@�C�����[�h���Ɉُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
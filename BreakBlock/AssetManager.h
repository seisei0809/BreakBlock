#pragma once
#include "AllSetting.h"

// �p�b�N���ꂽ�f�[�^�̏��
struct PackedFile {

	WSTR name;	        // �t�@�C���̖��O
	VECTOR<UINT8> data;	// �f�[�^���̂���
};

// �A�Z�b�g�Ǘ�
class AssetManager {
public:

	/// <summary>
	/// �p�b�N���ꂽ���̂����[�h���ĕԂ�
	/// </summary>
	/// <returns>�p�b�N���ꂽ���̂̔z��</returns>
	static VECTOR<PackedFile> LoadPack();

private:

	// �p�b�N�����t�@�C���̃p�X
	static WSTR _packPath;
};


#pragma once
#include "AllSetting.h"
#include "Sprite.h"

// ���_���
struct Vertex {

	XMFLOAT3 position;  // ���_�ʒu
	XMFLOAT2 uv;        // uv���W
};

// �萔�o�b�t�@�̃f�[�^�Ƃ��ăV�F�[�_�[�ɑ���\����
struct ConstantBufferData {

	XMMATRIX mat;   // ���W�ϊ��p�s��
	XMFLOAT4 color; // RGBA�̐F���
};

// �S�Ă�Sprite�Ǘ�
class SpriteObjectManager {
public:

	/// <summary>
	/// Sprite��z��ɒǉ�����
	/// </summary>
	/// <param name="">Sprite�{��</param>
	static void AddSprite(Sprite*);

	/// <summary>
	/// Sprite��j������
	/// </summary>
	/// <param name="sprite">Sprite�{��</param>
	static void DeleteSprite(Sprite*);

	/// <summary>
	/// Sprite���Ƃ�Draw�����ɌĂяo���Ă���
	/// </summary>
	static void Draw();

	/// <summary>
	/// Sprite��j������
	/// </summary>
	static void AllDeleteSprite();

private:

	// Sprite�ۑ��z��
	static VECTOR<Sprite*> _sprites;
};


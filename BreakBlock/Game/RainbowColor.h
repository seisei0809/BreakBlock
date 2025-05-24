#pragma once
#include "../AllSetting.h"

// ���F�ω�������
class RainbowColor {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="speed">���x</param>
	RainbowColor(float);

	/// <summary>
	/// �Ăяo���ƐF��ω�������
	/// </summary>
	void Update();

	/// <summary>
	/// �F���擾����
	/// </summary>
	/// <returns>���݂̐F</returns>
	inline XMFLOAT4 GetColor() { return { r, g, b, 1.0f }; }

private:

	// ��
	FLOAT r;
	// ��
	FLOAT g;
	// ��
	FLOAT b;
	// �ω����x
	FLOAT speed;
	// ���
	INT state;

};


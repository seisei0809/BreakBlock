#pragma once
#include "AllSetting.h"

// �f�o�b�O�֌W
class Debugger
{
public:

	/// <summary>
	/// DirectX�֌W�̃f�o�b�O�o�͂�����
	/// </summary>
	static void EnableDebugLayer();

	/// <summary>
	/// �����̐������s��MessageBox�ŏo��
	/// </summary>
	/// <param name="">���������s�����f������HRESULT</param>
	/// <param name="">�L���v�V�����ɂǂ̂悤�ɏo����</param>
	static void ShowHRESULT(HRESULT, const WSTR & = L"HRESULT");

	/// <summary>
	/// �������������Ă��邩�ǂ���
	/// </summary>
	/// <param name="">���������s������</param>
	/// <param name="">���s�������ɏo�����b�Z�[�W������</param>
	static void CheckHR(HRESULT, STR);
};


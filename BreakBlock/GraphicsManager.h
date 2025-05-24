#pragma once
#include "AllSetting.h"

// ��ʕ`��֌W
class GraphicsManager
{
public:

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̏�����
	/// </summary>
	static void InitGraphics();

	/// <summary>
	/// �`��
	/// </summary>
	static void Render();

	/// <summary>
	/// �O���t�B�b�N�X�̃����[�X
	/// </summary>
	static void ReleaseGraphics();

private:

	/// <summary>
	/// �T�C�Y�ɍ��킹�ăA���C�����g����
	/// </summary>
	/// <param name="size">�A���C�����g�������T�C�Y</param>
	/// <param name="alignment">�A���C�����g�{��</param>
	/// <returns>�A���C�����g��T�C�Y</returns>
	inline static ULONGLONG AligmentedSize(ULONGLONG size, ULONGLONG alignment) {
		return (size + alignment - 1) & ~(alignment - 1);
	}

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ID3D12PipelineState* _pipelinestate;

	// ���[�g�V�O�l�`��
	static ID3D12RootSignature* _rootsignature;

	// ���_�V�F�[�_�[�o�C�i��
	static ID3DBlob* _vsBlob;

	// �s�N�Z���V�F�[�_�[�o�C�i��
	static ID3DBlob* _psBlob;

	// �G���[�o�C�i��
	static ID3DBlob* _errorBlob;

	// �r���[�|�[�g
	static D3D12_VIEWPORT _viewport;

	// �V�U�[��`
	static D3D12_RECT _scissorrect;
};


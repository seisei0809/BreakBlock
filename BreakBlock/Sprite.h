#pragma once
#include "AllSetting.h"

// �萔�o�b�t�@�ɓn���f�[�^
struct ConstBufferData {

	XMMATRIX mat;   // �s��4x4
	XMFLOAT4 color; // RGBA�̐F���
};

// �ʒu��]�X�P�[�����O������
struct Transform {

	// �ʒu���W
	XMFLOAT3 position;

	// ��]���W
	FLOAT rotation;

	// �X�P�[�����O���W
	XMFLOAT2 scale;
};

// Sprite�{��
class Sprite
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="path">�p�X</param>
	/// <param name="fence">��p�̃t�F���X������Ȃ�</param>
	/// <param name="fenceVal">��p�̃t�F���X�������̃G�C���A�X</param>
	Sprite(WSTR, ID3D12Fence* = nullptr, UINT64 & = fenceValDefault);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Sprite();

	/// <summary>
	/// �Q�[���I�����ɃL���b�V������p
	/// </summary>
	static void CacheDelete();

	/// <summary>
	/// Transform�v�f�𒼂ɓn��
	/// </summary>
	/// <returns>Transform�{��</returns>
	inline Transform& GetTransform() { return transform; }

	/// <summary>
	/// Color�v�f�𒼂ɓn��
	/// </summary>
	/// <returns></returns>
	inline XMFLOAT4& GetColor() { return color; }

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`�悵�Ă��������̃`�F�b�N
	/// </summary>
	/// <returns>�A�N�e�B�u�Ȃ�true</returns>
	inline BOOL GetActive() const { return isActive; }

	/// <summary>
	/// �`�悷�邩�ǂ����̕ύX
	/// </summary>
	/// <param name="active">�A�N�e�B�u���</param>
	inline void SetActive(BOOL active) { isActive = active; }

private:

	// �A���C�����g�̌v�Z�ɕK�v
	ULONGLONG AligmentedSize(ULONGLONG size, ULONGLONG alignment);

	// �ʒu��]�X�P�[�����O��������Transform
	Transform transform;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;

	// ���_�o�b�t�@
	ID3D12Resource* vertBuff;

	// ���_�o�b�t�@�A�b�v���[�h�p
	ID3D12Resource* uploadBuff;

	// �C���f�b�N�X�o�b�t�@
	ID3D12Resource* idxBuff;

	// �e�N�X�`���o�b�t�@
	ID3D12Resource* texBuff;

	// �萔�o�b�t�@
	ID3D12Resource* constBuff;

	// SRV��CBV���w���f�B�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* descriHeap;

	// �A�b�v���[�h�p�A���P�[�^
	ID3D12CommandAllocator* uploadAllocator;

	// �A�b�v���[�h�p�R�}���h���X�g
	ID3D12GraphicsCommandList* uploadCmdList;

	// �A�b�v���[�h�p�R�}���h�L���[
	ID3D12CommandQueue* uploadCmdQueue;

	// ��p�t�F���X
	ID3D12Fence* fence;

	// ��p�̃t�F���X�̊�����
	UINT64& fenceVal;

	// �f�t�H���g�����p
	static UINT64 fenceValDefault;

	// �F���
	XMFLOAT4 color;

	// �`�悵�Ă�������
	BOOL isActive;

	// �摜�̖��O
	WSTR imageName;

	// �d���h�~�̂��߂̃e�N�X�`���L���b�V��
	static std::map<std::wstring, ID3D12Resource*> texBuffCache;

	// �d���h�~�̂��߂̃��^�f�[�^�L���b�V��
	static std::map<std::wstring, TexMetadata> metadataCache;

	// �d���h�~�̂��߂̉摜�{�̃L���b�V��
	static std::map<std::wstring, ScratchImage> scratchImgCache;
};


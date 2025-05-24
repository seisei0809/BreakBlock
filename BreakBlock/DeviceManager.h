#pragma once
#include "AllSetting.h"

// ������Device�֌W���Ǘ�
class DeviceManager
{
public:

	/// <summary>
	/// �E�B���h�E�̏�����
	/// </summary>
	static void InitWindow();

	/// <summary>
	/// �f�o�C�X�̏�����
	/// </summary>
	static void InitDevice();

	/// <summary>
	/// �f�o�C�X�̃����[�X
	/// </summary>
	static void ReleaseDevice();

	/// <summary>
	/// �f�o�C�X�̎擾
	/// </summary>
	/// <returns>�f�o�C�X�{��</returns>
	inline static ID3D12Device* GetDevice() { return _dev; }

	/// <summary>
	/// DXGI�̎擾
	/// </summary>
	/// <returns>DXGI�{��</returns>
	inline static IDXGIFactory6* GetDXGI() { return _dxgiFactory; }

	/// <summary>
	/// �X���b�v�`�F�[���̎擾
	/// </summary>
	/// <returns>�X���b�v�`�F�[���{��</returns>
	inline static IDXGISwapChain4* GetSwapChain() { return _swapchain; }

	/// <summary>
	/// �R�}���h�A���P�[�^�̎擾
	/// </summary>
	/// <returns>�R�}���h�A���P�[�^�{��</returns>
	inline static ID3D12CommandAllocator* GetCMDAllocator() { return _cmdAllocator; }

	/// <summary>
	/// �R�}���h���X�g�̎擾
	/// </summary>
	/// <returns>�R�}���h���X�g�{��</returns>
	inline static ID3D12GraphicsCommandList* GetCMDList() { return _cmdList; }

	/// <summary>
	/// �R�}���h�L���[�̎擾
	/// </summary>
	/// <returns>�R�}���h�L���[�{��</returns>
	inline static ID3D12CommandQueue* GetCMDQueue() { return _cmdQueue; }

	/// <summary>
	/// RTV�q�[�v�̈�̎擾
	/// </summary>
	/// <returns>RTV�q�[�v�{��</returns>
	inline static ID3D12DescriptorHeap* GetRTVHeap() { return _rtvHeap; }

	/// <summary>
	/// �t�F���X�̎擾
	/// </summary>
	/// <param name="i">�C���f�b�N�X</param>
	/// <returns>�t�F���X�{��</returns>
	inline static ID3D12Fence* GetFence(INT i) { return _fences[i]; }

	/// <summary>
	/// �t�F���X�̒l�̎擾
	/// </summary>
	/// <param name="i">�C���f�b�N�X</param>
	/// <returns>�t�F���X�̒l�{��</returns>
	inline static UINT64& GetFenceVal(INT i) { return _fenceVals[i]; }

	/// <summary>
	/// RTV�p�o�b�N�o�b�t�@�̎擾
	/// </summary>
	/// <param name="index">�C���f�b�N�X</param>
	/// <returns>RTV�p�o�b�N�o�b�t�@�{��</returns>
	inline static ID3D12Resource* GetBackBuffer(INT index) { return _backBuffers[index]; }

private:

	// Direct3D�̃f�o�C�X�i�[�p
	static ID3D12Device* _dev;

	// DXGI�i�[�p
	static IDXGIFactory6* _dxgiFactory;

	// �X���b�v�`�F�[������p
	static IDXGISwapChain4* _swapchain;

	// �R�}���h�A���P�[�^�i�[�p
	static ID3D12CommandAllocator* _cmdAllocator;

	// �R�}���h���X�g����p
	static ID3D12GraphicsCommandList* _cmdList;

	// �R�}���h�L���[����p
	static ID3D12CommandQueue* _cmdQueue;

	// RTV�̃q�[�v�̈�i�[�p
	static ID3D12DescriptorHeap* _rtvHeap;

	// �����������i�[�p  
	static VECTOR<UINT64> _fenceVals;

	// �t�F���X���Ă�p
	static VECTOR<ID3D12Fence*> _fences;

	// RTV�p�o�b�N�o�b�t�@�i�[�p
	static VECTOR<ID3D12Resource*> _backBuffers;
};


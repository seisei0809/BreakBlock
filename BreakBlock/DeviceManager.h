#pragma once
#include "AllSetting.h"

// ここでDevice関係を管理
class DeviceManager
{
public:

	/// <summary>
	/// ウィンドウの初期化
	/// </summary>
	static void InitWindow();

	/// <summary>
	/// デバイスの初期化
	/// </summary>
	static void InitDevice();

	/// <summary>
	/// デバイスのリリース
	/// </summary>
	static void ReleaseDevice();

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>デバイス本体</returns>
	inline static ID3D12Device* GetDevice() { return _dev; }

	/// <summary>
	/// DXGIの取得
	/// </summary>
	/// <returns>DXGI本体</returns>
	inline static IDXGIFactory6* GetDXGI() { return _dxgiFactory; }

	/// <summary>
	/// スワップチェーンの取得
	/// </summary>
	/// <returns>スワップチェーン本体</returns>
	inline static IDXGISwapChain4* GetSwapChain() { return _swapchain; }

	/// <summary>
	/// コマンドアロケータの取得
	/// </summary>
	/// <returns>コマンドアロケータ本体</returns>
	inline static ID3D12CommandAllocator* GetCMDAllocator() { return _cmdAllocator; }

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns>コマンドリスト本体</returns>
	inline static ID3D12GraphicsCommandList* GetCMDList() { return _cmdList; }

	/// <summary>
	/// コマンドキューの取得
	/// </summary>
	/// <returns>コマンドキュー本体</returns>
	inline static ID3D12CommandQueue* GetCMDQueue() { return _cmdQueue; }

	/// <summary>
	/// RTVヒープ領域の取得
	/// </summary>
	/// <returns>RTVヒープ本体</returns>
	inline static ID3D12DescriptorHeap* GetRTVHeap() { return _rtvHeap; }

	/// <summary>
	/// フェンスの取得
	/// </summary>
	/// <param name="i">インデックス</param>
	/// <returns>フェンス本体</returns>
	inline static ID3D12Fence* GetFence(INT i) { return _fences[i]; }

	/// <summary>
	/// フェンスの値の取得
	/// </summary>
	/// <param name="i">インデックス</param>
	/// <returns>フェンスの値本体</returns>
	inline static UINT64& GetFenceVal(INT i) { return _fenceVals[i]; }

	/// <summary>
	/// RTV用バックバッファの取得
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns>RTV用バックバッファ本体</returns>
	inline static ID3D12Resource* GetBackBuffer(INT index) { return _backBuffers[index]; }

private:

	// Direct3Dのデバイス格納用
	static ID3D12Device* _dev;

	// DXGI格納用
	static IDXGIFactory6* _dxgiFactory;

	// スワップチェーン操作用
	static IDXGISwapChain4* _swapchain;

	// コマンドアロケータ格納用
	static ID3D12CommandAllocator* _cmdAllocator;

	// コマンドリスト操作用
	static ID3D12GraphicsCommandList* _cmdList;

	// コマンドキュー操作用
	static ID3D12CommandQueue* _cmdQueue;

	// RTVのヒープ領域格納用
	static ID3D12DescriptorHeap* _rtvHeap;

	// 完了処理数格納用  
	static VECTOR<UINT64> _fenceVals;

	// フェンス立てる用
	static VECTOR<ID3D12Fence*> _fences;

	// RTV用バックバッファ格納用
	static VECTOR<ID3D12Resource*> _backBuffers;
};


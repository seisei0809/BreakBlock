#include "AllSetting.h"
#include "DeviceManager.h"
#include "Debugger.h"

// staticメンバの実体を定義

ID3D12Device* DeviceManager::_dev = nullptr;
IDXGIFactory6* DeviceManager::_dxgiFactory = nullptr;
IDXGISwapChain4* DeviceManager::_swapchain = nullptr;
ID3D12CommandAllocator* DeviceManager::_cmdAllocator = nullptr;
ID3D12GraphicsCommandList* DeviceManager::_cmdList = nullptr;
ID3D12CommandQueue* DeviceManager::_cmdQueue = nullptr;
ID3D12DescriptorHeap* DeviceManager::_rtvHeap = nullptr;
VECTOR<ID3D12Fence*> DeviceManager::_fences;
VECTOR<UINT64> DeviceManager::_fenceVals;
VECTOR<ID3D12Resource*> DeviceManager::_backBuffers;

void DeviceManager::InitWindow() {

	try
	{
		WNDCLASSEX w = {};

		w.cbSize = sizeof(WNDCLASSEX);							// バッファの確保
		w.lpfnWndProc = (WNDPROC)WinProc;						// コールバックする関数の指定
		w.lpszClassName = g_appName;							// ウィンドウクラスの名前の指定
		w.hInstance = GetModuleHandle(nullptr);					// 現在実行中のアプリケーション本体
		w.hIcon = LoadIcon(w.hInstance, (LPCTSTR)IDI_ICON1);	// アイコンの設定

		// クラス登録
		if (!RegisterClassEx(&w))
		{
			throw std::runtime_error(STR("ウィンドウクラスの登録ができませんでした"));
		}

		RECT wrc = { 0, 0, window_width, window_height };				// ウィンドウサイズを決める
		DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;	// タイトルとバツボタンだけ
		AdjustWindowRect(&wrc, windowStyle, FALSE);						// スタイルに合わせて調整

		// ウィンドウオブジェクトの作成
		HWND hwnd = CreateWindow(w.lpszClassName,   // クラス名指定
			g_appName,								// タイトルバーの文字
			windowStyle,
			CW_USEDEFAULT,                          // 表示x座標(OSの任せる)
			CW_USEDEFAULT,                          // 表示y座標(OSの任せる)
			wrc.right - wrc.left,                   // 幅
			wrc.bottom - wrc.top,                   // 高さ
			nullptr, nullptr, w.hInstance, nullptr);

		// 正しく作成できなかったら
		if (!hwnd) {

			throw std::runtime_error(STR("ウィンドウを作成できませんでした"));
		}

		g_hwnd = hwnd;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("ウィンドウの作成中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void DeviceManager::InitDevice() {

	try
	{
		// Direct3D 12のデバイス本体（GPUとやりとりする最も基本的なオブジェクト）
		_dev = nullptr;

		// DXGI（DirectX Graphics Infrastructure）のファクトリオブジェクト
		// 後でアダプター列挙やスワップチェイン作成などに使う
		_dxgiFactory = nullptr;

		// スワップチェイン（バックバッファと画面を入れ替えるオブジェクト）
		_swapchain = nullptr;

#ifdef _DEBUG
		// DXGIをデバッグタイプで作成
		g_hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_dxgiFactory));
		// デバッグレイヤーの有効化
		Debugger::EnableDebugLayer();
#else
		// DXGIの作成
		g_hr = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory));
#endif
		// サポートする機能レベルを上位から順に並べた配列（上位レベルが使えればそちらを優先）
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,  // 最新の機能レベル（最も高度）
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0   // 最低限必要な機能レベル
		};

		IDXGIAdapter1* adapter = nullptr;
		IDXGIAdapter1* bestAdapter = nullptr;
		ULONGLONG maxVRAM = 0;

		// アダプターを列挙して、最もVRAMが大きいアダプターを選択
		// EnumAdapters1()で各アダプターを列挙
		for (UINT i = 0; _dxgiFactory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			// ソフトウェアアダプターはスキップ
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				SAFE_RELEASE(adapter);  // メモリ解放
				continue;  // 次のアダプターを調べる
			}

			// 最も大きなVRAMを持つアダプターを選択
			if (desc.DedicatedVideoMemory > maxVRAM)
			{
				maxVRAM = desc.DedicatedVideoMemory;

				// 既に選ばれているアダプターを解放し、新しいアダプターを選定
				if (bestAdapter) SAFE_RELEASE(bestAdapter);
				SAFE_MOVE(adapter, bestAdapter);
				continue;
			}

			// 現在のアダプターを解放（次のアダプターのチェックに進む）
			adapter->Release();
		}

		// 最終的に選ばれたアダプターの情報を表示
		if (bestAdapter)
		{
			DXGI_ADAPTER_DESC1 desc;
			bestAdapter->GetDesc1(&desc);
#ifdef _DEBUG
			// デバッグ用出力
			OutputDebugStringW(L"選ばれたアダプター: ");
			OutputDebugStringW(desc.Description);
			OutputDebugStringW(L"\n");
#endif
		}

		// 利用可能な機能レベルを順番に試して、デバイスを作成
		for (auto lv : levels)
		{
			// デバイス作成を試みる
			g_hr = D3D12CreateDevice(
				bestAdapter,            // nullprtの場合、自動でドライバを選択される
				lv,                     // このドライバーを使う
				IID_PPV_ARGS(&_dev)     // 受け取りたいオブジェクトの型を示す
			);

			// 成功したらその機能レベルで確定してループを抜ける
			if (SUCCEEDED(g_hr))
				break;
		}

		// どの機能レベルでもデバイス作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "デバイスの作成に失敗しました");

		SAFE_RELEASE(bestAdapter);

		// コマンドアロケータ：コマンドリストが使うメモリ領域（命令を蓄積する場所）
		_cmdAllocator = nullptr;

		// コマンドリスト：GPUに送る描画・状態設定などの命令を記録するためのインターフェース
		_cmdList = nullptr;

		// コマンドアロケータを作成（DIRECTタイプは描画用の命令列を意味する）
		g_hr = _dev->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,  // 描画命令に対応するタイプ
			IID_PPV_ARGS(&_cmdAllocator));   // 結果を格納するポインタ

		// コマンドアロケータの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドアロケータの作成に失敗しました");

		// コマンドリストを作成
		g_hr = _dev->CreateCommandList(
			0,                               // ノードインデックス（マルチGPUでなければ 0）
			D3D12_COMMAND_LIST_TYPE_DIRECT, // DIRECT：描画・コピーなど標準的な命令
			_cmdAllocator,                  // 使用するコマンドアロケータ
			nullptr,                        // 初期化時にバインドするパイプラインステート
			IID_PPV_ARGS(&_cmdList));       // 結果を格納するポインタ

		// コマンドリストの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドリストの作成に失敗しました");

		// いったん閉じる
		g_hr = _cmdList->Close();

		// コマンドリストを閉じるのに失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドリストを閉じるのに失敗しました");

		// コマンドキュー：GPUに命令を送るための列。複数のコマンドリストをまとめて送るためのもの。
		_cmdQueue = nullptr;

		// コマンドキューの設定情報を保持する構造体（初期化）
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;              // 特別な動作フラグは使わない
		cmdQueueDesc.NodeMask = 0;                                       // 単一GPU環境では常に0
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;     // 通常優先度
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;              // コマンドリストと合わせる

		// コマンドキューをデバイスから作成
		g_hr = _dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_cmdQueue));

		// コマンドキューの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドキューの作成に失敗しました");

		// スワップチェインの設定構造体（バージョン1）を初期化
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};

		swapchainDesc.Width = window_width;                              // ウィンドウの幅
		swapchainDesc.Height = window_height;                            // ウィンドウの高さ
		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;               // 標準的なRGBA8フォーマット（各色8bit、UNORM＝0.0〜1.0範囲）
		swapchainDesc.Stereo = false;                                    // VRのようなステレオ表示は使わない
		swapchainDesc.SampleDesc.Count = 1;                              // マルチサンプリングしない（＝1）
		swapchainDesc.SampleDesc.Quality = 0;                            // マルチサンプリング品質（0でOK）
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // このバッファは描画結果の出力先として使う
		swapchainDesc.BufferCount = 2;                                   // ダブルバッファリング（2つのバッファを交互に使う）

		swapchainDesc.Scaling = DXGI_SCALING_STRETCH;                    // ウィンドウサイズにストレッチ（伸縮）して表示

		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // Presentしたバックバッファの内容は破棄される。

		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;           // 透明度（α値）の扱いは特に指定しない

		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // Alt+Enterでのフルスクリーン切り替えを許可

		// ウィンドウに紐づくスワップチェインを作成
		g_hr = _dxgiFactory->CreateSwapChainForHwnd(
			_cmdQueue,                                                    // 使用するコマンドキュー（描画結果が出る場所）
			g_hwnd,                                                       // 関連づけるウィンドウハンドル
			&swapchainDesc,                                               // スワップチェインの詳細情報
			nullptr,                                                      // フルスクリーン設定（使わないのでnullptr）
			nullptr,                                                      // モニターの指定（使わないのでnullptr）
			(IDXGISwapChain1**)&_swapchain                                // 作成したスワップチェインの格納先（バージョン1経由で取得）
		);

		// スワップチェインの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "スワップチェインの作成に失敗しました");

		// D3D12 ディスクリプタヒープ（RTV用）の設定情報を作成
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

		// ヒープの種類をRTVに設定
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// RTVのディスクリプタを格納
		heapDesc.NodeMask = 0;								// ノードマスク（マルチGPU環境の場合の設定だが、通常は0で問題なし）
		heapDesc.NumDescriptors = 2;						// バッファの数分（表裏の2つ）
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 特にフラグはなし（共有やシェーダ可視設定などは行わない）

		// RTV用のディスクリプタヒープを作成する
		_rtvHeap = nullptr;
		g_hr = _dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_rtvHeap));

		// RTV用ディスクリプタヒープの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "RTV用ディスクリプタヒープの作成に失敗しました");

		// スワップチェインの設定情報を取得
		DXGI_SWAP_CHAIN_DESC swcDesc = { };
		g_hr = _swapchain->GetDesc(&swcDesc);

		// バックバッファの数をスワップチェインに合わせてリサイズ
		_backBuffers.resize(swcDesc.BufferCount);

		// ディスクリプタヒープ内で最初の位置を取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

		// SRGBレンダーターゲットビュー設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;		// ガンマ補正あり
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ専用RTV

		// バックバッファごとにRTVを作成
		for (UINT i = 0; i < swcDesc.BufferCount; i++) {

			// スワップチェインからバックバッファを取得
			g_hr = _swapchain->GetBuffer(i, IID_PPV_ARGS(&_backBuffers[i]));

			// バックバッファをRTVとして作成
			_dev->CreateRenderTargetView(
				_backBuffers[i],   // 対象バックバッファ
				&rtvDesc,            // 特別な設定がなければnullptr
				handle);            // ディスクリプタヒープの位置（格納先）

			// 次のディスクリプタ位置へ移動
			handle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		_fences.resize(2);
		_fenceVals.resize(2);

		for (INT i = 0; i < _fences.size(); i++)
		{
			// GPUとCPUの同期用のフェンスを作成
			_fences[i] = nullptr;
			_fenceVals[i] = 0;

			// フェンスオブジェクトを作成
			g_hr = _dev->CreateFence(_fenceVals[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fences[i]));

			// フェンスオブジェクトの作成に失敗したらthrow
			Debugger::CheckHR(g_hr, "フェンスオブジェクトの作成に失敗しました");
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(std::string("デバイスの作成中に異常エラー:") + e.what() + STR(":"));
	}
}

void DeviceManager::ReleaseDevice() {

	try
	{
		// GPUの処理が完了するまで待機
		if (_cmdQueue && !_fences.empty())
		{
			for (INT i = 0; i < _fences.size(); ++i)
			{
				if (_fences[i])
				{
					_cmdQueue->Signal(_fences[i], ++_fenceVals[i]);

					if (_fences[i]->GetCompletedValue() < _fenceVals[i])
					{
						HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
						_fences[i]->SetEventOnCompletion(_fenceVals[i], event);
						WaitForSingleObject(event, INFINITE);
						CloseHandle(event);
					}
				}
			}
		}

		if (_backBuffers.size()) {
			// バックバッファのリソースを解放
			for (auto& buffer : _backBuffers)
			{
				SAFE_RELEASE(buffer);
			}
			_backBuffers.clear();
		}

		if (_swapchain)		SAFE_RELEASE(_swapchain);
		if (_rtvHeap)		SAFE_RELEASE(_rtvHeap);
		if (_cmdQueue)		SAFE_RELEASE(_cmdQueue);
		if (_cmdList)		SAFE_RELEASE(_cmdList);
		if (_cmdAllocator)	SAFE_RELEASE(_cmdAllocator);
		for (INT i = 0; i < _fences.size(); ++i)
		{
			SAFE_RELEASE(_fences[i]);
		}
		if (_dxgiFactory)	SAFE_RELEASE(_dxgiFactory);
		if (_dev)			SAFE_RELEASE(_dev);

#ifdef _DEBUG
		IDXGIDebug* dxgiDebug = nullptr;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)))) {
			dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
			dxgiDebug->Release();
		}
#endif
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("デバイスのリリース中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}
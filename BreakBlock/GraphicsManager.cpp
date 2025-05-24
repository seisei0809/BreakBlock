#include "AllSetting.h"
#include "GraphicsManager.h"
#include "Debugger.h"
#include "DeviceManager.h"
#include "SpriteObjectManager.h"

// staticメンバの実体を定義

ID3D12PipelineState* GraphicsManager::_pipelinestate = nullptr;
ID3D12RootSignature* GraphicsManager::_rootsignature = nullptr;
ID3DBlob* GraphicsManager::_vsBlob = nullptr;
ID3DBlob* GraphicsManager::_psBlob = nullptr;
ID3DBlob* GraphicsManager::_errorBlob = nullptr;
D3D12_VIEWPORT GraphicsManager::_viewport = {};
D3D12_RECT GraphicsManager::_scissorrect = {};
D3D12_VERTEX_BUFFER_VIEW _vbView = {};
D3D12_INDEX_BUFFER_VIEW _ibView = {};

void GraphicsManager::InitGraphics() {

	try
	{
		// シェーダーバイナリ格納用
		_vsBlob = nullptr;
		_psBlob = nullptr;
		_errorBlob = nullptr;

		// 相対パスからコンパイル済みVSファイルを読み込む
		g_hr = D3DReadFileToBlob(L"Shaders/VertexShader.cso", &_vsBlob);

		// VSファイルの読み込みに失敗したらthrow
		Debugger::CheckHR(g_hr, "VSファイルの読み込みに失敗しました");

		// 相対パスからコンパイル済みPSファイルを読み込む
		g_hr = D3DReadFileToBlob(L"Shaders/PixelShader.cso", &_psBlob);

		// PSファイルの読み込みに失敗したらthrow
		Debugger::CheckHR(g_hr, "PSファイルの読み込みに失敗しました");

		// 頂点レイアウトの定義
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// パイプライン設定構造体の初期化
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
		gpipeline.pRootSignature = nullptr; // 後で設定

		// 頂点シェーダのバイナリ設定
		gpipeline.VS.pShaderBytecode = _vsBlob->GetBufferPointer();
		gpipeline.VS.BytecodeLength = _vsBlob->GetBufferSize();

		// ピクセルシェーダのバイナリ設定
		gpipeline.PS.pShaderBytecode = _psBlob->GetBufferPointer();
		gpipeline.PS.BytecodeLength = _psBlob->GetBufferSize();

		// サンプルマスクの設定（全ビットON）
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ラスタライザステートの設定
		gpipeline.RasterizerState.MultisampleEnable = false;               // マルチサンプル無効
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;         // カリング無効
		gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;        // 塗りつぶし描画
		gpipeline.RasterizerState.DepthClipEnable = true;                  // 深度クリッピング有効

		// ブレンドステートの設定
		gpipeline.BlendState.AlphaToCoverageEnable = false;                // アルファカバレッジ無効
		gpipeline.BlendState.IndependentBlendEnable = false;               // 独立ブレンド無効

		// 各レンダーターゲット用のブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		// ブレンドステートの設定
		renderTargetBlendDesc.BlendEnable = true;                                   // ブレンドを有効化
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;                     // ソースのアルファ値
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;                // 逆アルファ値
		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;                         // 加算ブレンド
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;                      // アルファ値のソース
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;                    // アルファ値のデスティネーション
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;                    // アルファ値の加算
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBAすべて描画

		// 設定を適用
		gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

		// 頂点レイアウトの設定
		gpipeline.InputLayout.pInputElementDescs = inputLayout;
		gpipeline.InputLayout.NumElements = ARRAYSIZE(inputLayout);

		// インデックスバッファのカット値設定（無効）
		gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

		// トポロジーの種類（三角形リスト）
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// レンダーターゲットの設定
		gpipeline.NumRenderTargets = 1;
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		// サンプリングの設定
		gpipeline.SampleDesc.Count = 1;
		gpipeline.SampleDesc.Quality = 0;

		D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};

		descTblRange[0].NumDescriptors = 1;                            // テクスチャは一つ
		descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;   // 種別はテクスチャ
		descTblRange[0].BaseShaderRegister = 0;                        // 0番スロットから
		descTblRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		descTblRange[1].NumDescriptors = 1;                            // 定数は一つ
		descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;   // 種別は定数
		descTblRange[1].BaseShaderRegister = 0;                        // 0番スロットから
		descTblRange[1].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER rootparam[2] = {};

		rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// 全てのシェーダーから見える
		rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];      // ディスクリプタレンジのアドレス
		rootparam[0].DescriptorTable.NumDescriptorRanges = 1;                   // ディスクリプタレンジ数
		rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// 全てのシェーダーから見える
		rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];      // ディスクリプタレンジのアドレス
		rootparam[1].DescriptorTable.NumDescriptorRanges = 1;                   // ディスクリプタレンジ数

		D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

		samplerDesc.ShaderRegister = 0;                             // 明示的にサンプラーの0スロットに送る
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;     // 横方向の繰り返し
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;     // 縦方向の繰り返し
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;     // 奥方向の繰り返し
		samplerDesc.BorderColor =
			D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;            // ボーダーは黒
		//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;       // 線形補完
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;        // 補完しない(ニアレストレイバー法)
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                     // ミップマップ最大値
		samplerDesc.MinLOD = 0.0f;                                  // ミップマップ最小値
		samplerDesc.ShaderVisibility =
			D3D12_SHADER_VISIBILITY_PIXEL;                          // ピクセルシェーダーから見える
		samplerDesc.ShaderRegister = 0;                             // デフォルトでスロット0
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;   // リサンプリングしない

		// ルートシグネチャの定義
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootparam; // ルートパラメータの先頭アドレス
		rootSignatureDesc.NumParameters = 2;        // ルートパラメータ数
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ルートシグネチャのシリアライズ
		ID3DBlob* rootSignatureBlob = nullptr;
		g_hr = D3D12SerializeRootSignature(
			&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &_errorBlob);

		// ルートシグネチャのシリアライズに失敗したらthrow
		Debugger::CheckHR(g_hr, "ルートシグネチャのシリアライズに失敗しました");

		// ルートシグネチャ作成
		_rootsignature = nullptr;
		g_hr = DeviceManager::GetDevice()->CreateRootSignature(
			0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&_rootsignature));

		// ルートシグネチャの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "ルートシグネチャの作成に失敗しました");

		// 使い終わったバイナリ解放
		rootSignatureBlob->Release();

		// パイプラインにルートシグネチャを設定
		gpipeline.pRootSignature = _rootsignature;

		// ビューポートの設定
		D3D12_VIEWPORT viewport = {};
		viewport.Width = (FLOAT)window_width;
		viewport.Height = (FLOAT)window_height;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		_viewport = viewport;

		// シザー矩形の設定
		D3D12_RECT scissorrect = {};
		scissorrect.top = 0;
		scissorrect.left = 0;
		scissorrect.right = scissorrect.left + window_width;
		scissorrect.bottom = scissorrect.top + window_height;
		_scissorrect = scissorrect;

		// パイプラインステート作成
		_pipelinestate = nullptr;
		g_hr = DeviceManager::GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&_pipelinestate));

		// パイプラインステートの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "パイプラインステートの作成に失敗しました");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("グラフィックス管理初期化中に異常エラー:") + e.what() + STR(":"));
	}
}

void GraphicsManager::Render() {

	try
	{
		// コマンドアロケータのリセット（新しいコマンドを記録できるようにする）
		g_hr = DeviceManager::GetCMDAllocator()->Reset();

		// コマンドアロケータのリセットに失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドアロケータのリセットに失敗しました");

		// コマンドリストのリセット（新しいコマンドを追加する準備）
		g_hr = DeviceManager::GetCMDList()->Reset(DeviceManager::GetCMDAllocator(), nullptr);

		// コマンドリストのリセットに失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドリストのリセットに失敗しました");

		// パイプラインステートの設定
		DeviceManager::GetCMDList()->SetPipelineState(_pipelinestate);

		// ルートシグネチャを設定
		DeviceManager::GetCMDList()->SetGraphicsRootSignature(_rootsignature);

		// 現在のバックバッファのインデックスを取得
		auto bbIdx = DeviceManager::GetSwapChain()->GetCurrentBackBufferIndex();

		// リソースバリアを設定して、リソースの状態遷移を行う準備
		D3D12_RESOURCE_BARRIER barrierDesc = {};

		// 遷移の種類としてリソースバリアのタイプを設定
		barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION; // 遷移タイプ
		barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; // 特にフラグは指定しない
		barrierDesc.Transition.pResource = DeviceManager::GetBackBuffer(bbIdx); // 遷移対象のリソース（バックバッファ）
		barrierDesc.Transition.Subresource = 0; // サブリソースは全体
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 遷移前の状態（PRESENT）
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 遷移後の状態（RENDER_TARGET）
		// リソースバリアをコマンドリストに追加
		DeviceManager::GetCMDList()->ResourceBarrier(1, &barrierDesc);

		// RTVヒープからCPU用のディスクリプタハンドルを取得
		auto rtvh = DeviceManager::GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

		// バックバッファに対応するRTVのディスクリプタ位置にオフセットを適用
		rtvh.ptr += (ULONGLONG)bbIdx * DeviceManager::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// レンダーターゲットを設定
		DeviceManager::GetCMDList()->OMSetRenderTargets(1, &rtvh, false, nullptr);

		// レンダーターゲットビューをクリア（背景色を塗る）
		DeviceManager::GetCMDList()->ClearRenderTargetView(rtvh, backColor, 0, nullptr);

		// ビューポートを設定
		DeviceManager::GetCMDList()->RSSetViewports(1, &_viewport);

		// スカシ矩形を設定
		DeviceManager::GetCMDList()->RSSetScissorRects(1, &_scissorrect);

		// トライアングルリストとしての構成とする
		DeviceManager::GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Spriteの描画をここで行う
		SpriteObjectManager::Draw();

		// 描画後のリソース状態遷移を再度設定（RENDER_TARGET → PRESENT）
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 遷移前の状態（RENDER_TARGET）
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 遷移後の状態（PRESENT）
		// 再度リソースバリアをコマンドリストに追加
		DeviceManager::GetCMDList()->ResourceBarrier(1, &barrierDesc);

		// コマンドリストを閉じて、コマンドの記録を終了
		g_hr = DeviceManager::GetCMDList()->Close();

		// コマンドリストを閉じるのに失敗したらthrow
		Debugger::CheckHR(g_hr, "コマンドリストを閉じるのに失敗しました");

		// コマンドリストを実行するためにリストを指定
		ID3D12CommandList* cmdlists[] = { DeviceManager::GetCMDList() };

		// コマンドキューにコマンドリストを追加して実行
		DeviceManager::GetCMDQueue()->ExecuteCommandLists(ARRAYSIZE(cmdlists), cmdlists);

		// 前回このバッファを使った時のGPU完了を待つ
		if (DeviceManager::GetFence(bbIdx)->GetCompletedValue() < DeviceManager::GetFenceVal(bbIdx)) {
			HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			DeviceManager::GetFence(bbIdx)->SetEventOnCompletion(DeviceManager::GetFenceVal(bbIdx), event);
			if (event != 0) {
				WaitForSingleObject(event, INFINITE);
				CloseHandle(event);
			}
		}

		// フェンスにシグナルを送る(GPUに処理が完了したことを知らせる)
		DeviceManager::GetCMDQueue()->Signal(DeviceManager::GetFence(bbIdx), ++DeviceManager::GetFenceVal(bbIdx));

		// スワップチェインの発動(バックバッファとフロントバッファを切り替える)
		// こちらでSleepしてるので垂直同期なし
		g_hr = DeviceManager::GetSwapChain()->Present(0, 0);
	}
	catch (const std::exception& e)
	{
		// あるならエラー出力
		if (_errorBlob) {

			OutputDebugStringA((char*)_errorBlob->GetBufferPointer());
		}
		throw std::runtime_error(STR("グラフィックス管理描画中に異常エラー:") + e.what() + STR(":"));
	}

}

void GraphicsManager::ReleaseGraphics() {

	try
	{
		if (_pipelinestate)    SAFE_RELEASE(_pipelinestate);
		if (_rootsignature)    SAFE_RELEASE(_rootsignature);
		if (_vsBlob)           SAFE_RELEASE(_vsBlob);
		if (_psBlob)           SAFE_RELEASE(_psBlob);
		if (_errorBlob)        SAFE_RELEASE(_errorBlob);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("グラフィックス管理リリース中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}
#include "AllSetting.h"
#include "Sprite.h"
#include "SpriteObjectManager.h"
#include "DeviceManager.h"
#include "Debugger.h"
#include "AssetManager.h"

// staticメンバの実体を定義

std::map<std::wstring, ID3D12Resource*> Sprite::texBuffCache;
std::map<std::wstring, TexMetadata> Sprite::metadataCache;
std::map<std::wstring, ScratchImage> Sprite::scratchImgCache;
UINT64 Sprite::fenceValDefault = 0;

Sprite::Sprite(WSTR path, ID3D12Fence* fence, UINT64& fenceVal)
	: transform({ {0, 0, 0}, 0.0f, {1, 1} }),   // 初期座標
	vbView(),
	ibView(),
	vertBuff(nullptr),
	uploadBuff(nullptr),
	idxBuff(nullptr),
	texBuff(nullptr),
	constBuff(nullptr),
	descriHeap(nullptr),
	color(1.0f, 1.0f, 1.0f, 1.0f),              // 白色
	isActive(true),                             // アクティブ状態
	imageName(path),
	uploadAllocator(nullptr),
	uploadCmdList(nullptr),
	uploadCmdQueue(nullptr),
	fence(fence),
	fenceVal(fenceVal)
{

	try
	{
		// 引数にフェンスがなければデフォルト
		if (!fence) {

			fence = DeviceManager::GetFence(0);
			fenceVal = DeviceManager::GetFenceVal(0);
		}

		// インデックスデータ
		static USHORT indices[] = { 0, 1, 2, 2, 1, 3 };

		// 最初はインデックスデータ用
		static D3D12_RESOURCE_DESC resDesc;
		resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices));   // リソースの設定
		static D3D12_HEAP_PROPERTIES heapProp;
		heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD); // ヒープの設定

		// インデックスバッファ作成
		g_hr = DeviceManager::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&idxBuff));

		// インデックスバッファの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "インデックスバッファの作成に失敗しました");

		// インデックスデータ転送
		static USHORT* mappedIdx = nullptr;
		g_hr = idxBuff->Map(0, nullptr, (void**)&mappedIdx);

		// インデックスバッファのマップに失敗したらthrow
		Debugger::CheckHR(g_hr, "インデックスバッファのマップに失敗しました");

		// 最初から最後までコピー
		std::copy(std::begin(indices), std::end(indices), mappedIdx);
		idxBuff->Unmap(0, nullptr);

		// インデックスバッファビュー作成
		ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeof(indices);

		// 頂点の一時変数  
		static Vertex vertices[4];

		// 頂点をセット
		vertices[0] = { {0.0f,1.0f,0.0f}, { 0.0f, 1.0f } };
		vertices[1] = { {0.0f,0.0f,0.0f},{ 0.0f, 0.0f } };
		vertices[2] = { {1.0f,1.0f,0.0f},{ 1.0f, 1.0f } };
		vertices[3] = { {1.0f,0.0f,0.0f},{ 1.0f, 0.0f } };

		// 頂点バッファ用に書き換える
		resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));

		// 頂点バッファ作成
		g_hr = DeviceManager::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));

		// 頂点バッファの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "頂点バッファの作成に失敗しました");

		// 頂点バッファビュー作成
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeof(vertices);
		vbView.StrideInBytes = sizeof(Vertex);

		//メタデータ(画像の情報)
		TexMetadata metadata = {};
		// 画像自体
		ScratchImage scratchImg = {};

		// キャッシュからロードしたかどうか
		bool loadedFromCache = false;
		// キャッシュに既にあるかどうか
		auto it = texBuffCache.find(imageName);
		if (it != texBuffCache.end()) {

			// キャッシュにあったので使い回す
			texBuff = it->second;
			metadata = metadataCache[imageName];
			scratchImg = std::move(scratchImgCache[imageName]);
			// ロードしたことにする
			loadedFromCache = true;
		}

		// キャッシュからロードしたならスキップ
		if (!loadedFromCache) {

			// パックされたファイルのロード
			static VECTOR<PackedFile> files = AssetManager::LoadPack();

			// forカウント用
			INT count = 0;
			// 展開した中から探す
			for (auto& file : files) {

				count++;
				// 名前と一致したら
				if (file.name == imageName) {

					// WIC画像としてロードする
					g_hr = LoadFromWICMemory(
						file.data.data(),                  // メモリ上のデータ先頭
						file.data.size(),                  // サイズ
						WIC_FLAGS_NONE,                    // フラグ
						&metadata,                         // メタデータ格納先
						scratchImg                         // イメージ格納先
					);

					// WIC画像のロードに失敗したらthrow
					Debugger::CheckHR(g_hr, "WIC画像のロードに失敗しました");

					break;
				}

				// 最後まで行ったらthrow
				if (files.size() == count) {

					throw std::runtime_error(std::string("画像が見つかりませんでした:"));
				}
			}



			// 画像の抽出用
			static const Image* img;
			// 画像の抽出
			img = scratchImg.GetImage(0, 0, 0);

			// ピクセルサイズ(B8G8R8A8_UNORM_SRGBは1ピクセルあたり4バイトなので)
			static size_t pixelSize = 4;

			// 行ごとのサイズ
			static size_t rowPitch;
			// 256ずつが都合がいいのでアライメント
			rowPitch = AligmentedSize(img->width * pixelSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

			// テクスチャバッファ用にする
			resDesc.Width = rowPitch * img->height;

			// 中間バッファの作成
			g_hr = DeviceManager::GetDevice()->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadBuff)
			);

			// アップロードバッファの作成に失敗したらthrow
			Debugger::CheckHR(g_hr, "アップロードバッファの作成に失敗しました");

			// ピクセル情報をアップロードバッファに送る
			static UINT8* mapforImg = nullptr;
			g_hr = uploadBuff->Map(0, nullptr, (void**)&mapforImg);

			// アップロードバッファのマップに失敗したらthrow
			Debugger::CheckHR(g_hr, "アップロードバッファのマップに失敗しました");

			// ピクセルデータのアドレス
			static UINT8* srcAddres;
			srcAddres = img->pixels;

			// 1行ずつやるので高さまで
			for (INT y = 0; y < img->height; y++) {

				// 今の行に行分コピーする
				std::copy_n(srcAddres,
					rowPitch,
					mapforImg);

				// 一行ごとのつじつまを合わせる
				srcAddres += img->rowPitch;
				mapforImg += rowPitch;
			}
			uploadBuff->Unmap(0, nullptr);

			// テクスチャバッファ用にする
			heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			resDesc.Format = metadata.format;
			resDesc.Width = (UINT64)metadata.width;
			resDesc.Height = (UINT)metadata.height;
			resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
			resDesc.MipLevels = (UINT16)metadata.mipLevels;
			resDesc.Dimension = (D3D12_RESOURCE_DIMENSION)(metadata.dimension);
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

			// テクスチャバッファの作成
			g_hr = DeviceManager::GetDevice()->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // テクスチャ用指定
				nullptr,
				IID_PPV_ARGS(&texBuff)
			);

			// コピー元設定
			D3D12_TEXTURE_COPY_LOCATION src = {};
			src.pResource = uploadBuff;
			src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			src.PlacedFootprint.Offset = 0;
			src.PlacedFootprint.Footprint.Width = (UINT)img->width;
			src.PlacedFootprint.Footprint.Height = (UINT)img->height;
			src.PlacedFootprint.Footprint.Depth = 1;
			src.PlacedFootprint.Footprint.Format = metadata.format;
			src.PlacedFootprint.Footprint.RowPitch = (UINT)(rowPitch);


			D3D12_TEXTURE_COPY_LOCATION dst = {};

			// コピー先設定
			dst.pResource = texBuff;
			dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dst.SubresourceIndex = 0;

			// リソースバリアを設定(コピー用)
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				texBuff,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_COPY_DEST
			);

			// コマンドアロケータ作成
			g_hr = DeviceManager::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&uploadAllocator));
			Debugger::CheckHR(g_hr, "アップロード用コマンドアロケータの作成に失敗");

			// コマンドリスト作成
			g_hr = DeviceManager::GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, uploadAllocator, nullptr, IID_PPV_ARGS(&uploadCmdList));
			Debugger::CheckHR(g_hr, "アップロード用コマンドリストの作成に失敗");

			// コマンドキュー作成
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			g_hr = DeviceManager::GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&uploadCmdQueue));
			Debugger::CheckHR(g_hr, "アップロード用コマンドキューの作成に失敗");

			// リセット
			uploadAllocator->Reset();

			// リセット
			uploadCmdList->Reset(uploadAllocator, nullptr);

			// リソースバリアしとく
			uploadCmdList->ResourceBarrier(1, &barrier);

			// テクスチャバッファにUpload用の内容をコピーする
			uploadCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

			// 次はVSに送る準備をする
			barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				texBuff,
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			);

			// リソースバリアしとく
			uploadCmdList->ResourceBarrier(1, &barrier);

			uploadCmdList->Close();

			// コマンドリストの実行
			ID3D12CommandList* cmdlists[] = { uploadCmdList };
			uploadCmdQueue->ExecuteCommandLists(1, cmdlists);

			uploadCmdQueue->Signal(fence, ++fenceVal);
		}

		// キャッシュする
		texBuffCache[imageName] = texBuff;
		metadataCache[imageName] = metadata;
		scratchImgCache[imageName] = std::move(scratchImg);

		// 行列の初期化
		static XMMATRIX matrix = XMMatrixIdentity();
		// 行列に座標変換用として設定する
		matrix.r[0].m128_f32[0] = 2.0f / window_width;
		matrix.r[1].m128_f32[1] = -2.0f / window_height;
		matrix.r[0].m128_f32[3] = -1;
		matrix.r[1].m128_f32[3] = 1;

		// 色初期化
		static XMFLOAT4 color = { 1,1,1,1 };

		// 定数バッファデータの初期化
		static ConstantBufferData cbd;
		cbd = { matrix,color };

		// 行列用ヒープ設定
		static D3D12_HEAP_PROPERTIES heapPropMat = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// 行列用リソース設定
		static D3D12_RESOURCE_DESC resDescMat = CD3DX12_RESOURCE_DESC::Buffer((sizeof(cbd) + 0xff) & ~0xff);

		// 行列のための定数バッファの作成
		g_hr = DeviceManager::GetDevice()->CreateCommittedResource(
			&heapPropMat,
			D3D12_HEAP_FLAG_NONE,
			&resDescMat,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);

		// 定数バッファの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "定数バッファの作成に失敗しました");

		// マップ先を示すポインタ
		static ConstantBufferData* mapCBD;
		// マップする
		g_hr = constBuff->Map(0, nullptr, (void**)&mapCBD);

		// 定数バッファのマップに失敗したらthrow
		Debugger::CheckHR(g_hr, "定数バッファのマップに失敗しました");

		// 内容のコピー
		*mapCBD = cbd;

		// ディスクリプタヒープ作成(2個分)
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 2;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		g_hr = DeviceManager::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriHeap));

		// ディスクリプタヒープの作成に失敗したらthrow
		Debugger::CheckHR(g_hr, "ディスクリプタヒープの作成に失敗しました");

		// ディスクリプタヒープの中のハンドル
		static D3D12_CPU_DESCRIPTOR_HANDLE handle;

		// 最初を指す
		handle = descriHeap->GetCPUDescriptorHandleForHeapStart();

		// SRV作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = metadata.format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		DeviceManager::GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, handle);

		// CBV作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
		handle.ptr += DeviceManager::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		DeviceManager::GetDevice()->CreateConstantBufferView(&cbvDesc, handle);

		// Sprite管理に入れる
		SpriteObjectManager::AddSprite(this);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Spriteコンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

Sprite::~Sprite() {

	// uploadBuffがロード中かどうか見るため
	// フェンスが完了するまで待機
	if (fence && fence->GetCompletedValue() < fenceVal) {
		// 完了イベントを作成
		auto event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		// フェンスが指定された値に到達したらイベントを発生させる
		fence->SetEventOnCompletion(fenceVal, event);

		if (event != 0) {
			// イベントが発生するまで待機
			WaitForSingleObject(event, INFINITE);

			// イベントハンドルを閉じる（リソースを解放）
			CloseHandle(event);
		}
	}

	if (fence)			fence = nullptr;
	if (uploadCmdQueue)	SAFE_RELEASE(uploadCmdQueue);
	if (uploadCmdList)	SAFE_RELEASE(uploadCmdList);
	if (uploadAllocator)SAFE_RELEASE(uploadAllocator);
	if (uploadBuff)		SAFE_RELEASE(uploadBuff);
	if (vertBuff)		SAFE_RELEASE(vertBuff);
	if (idxBuff)		SAFE_RELEASE(idxBuff);
	if (constBuff)		SAFE_RELEASE(constBuff);
	if (descriHeap)		SAFE_RELEASE(descriHeap);
}

void Sprite::CacheDelete() {

	try
	{
		// テクスチャバッファの解放
		for (auto it = texBuffCache.begin(); it != texBuffCache.end(); ++it) {
			SAFE_RELEASE(it->second);
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Spriteキャッシュ破棄で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void Sprite::Draw() {

	try
	{
		// uploadBuffがロード中かどうか見るため
		// まだなら描画スキップ
		if (fence && fence->GetCompletedValue() < fenceVal) return;

		// 頂点の一時変数  
		static Vertex vertices[4];

		// 回転
		static float angle;
		angle = transform.rotation;

		// 回転行列を計算
		static XMMATRIX rotMat;
		rotMat = XMMatrixRotationZ(angle);

		// 平行移動用のオフセット
		static XMVECTOR offset;
		offset = XMVectorSet(transform.position.x, transform.position.y, 0.0f, 0.0f);

		// オフセット前のローカル座標
		XMFLOAT2 localPositions[4] =
		{
			{ -transform.scale.x,  transform.scale.y }, // 左下
			{ -transform.scale.x, -transform.scale.y }, // 左上
			{  transform.scale.x,  transform.scale.y }, // 右下
			{  transform.scale.x, -transform.scale.y }  // 右上
		};

		// 頂点ごとに回転＋移動処理
		for (INT i = 0; i < 4; ++i)
		{
			// ローカル座標のXMVECTOR化
			static XMVECTOR localPos;
			localPos = XMVectorSet(localPositions[i].x, localPositions[i].y, 0.0f, 1.0f);

			// 回転行列の適用結果
			static XMVECTOR rotatedPos;
			rotatedPos = XMVector4Transform(localPos, rotMat);

			// オフセットの加算
			rotatedPos = XMVectorAdd(rotatedPos, offset);

			// 結果をXMFLOAT3に格納（GPU用頂点データ）
			XMStoreFloat3(&vertices[i].position, rotatedPos);
		}

		// UVをセット
		vertices[0].uv = { 0.0f, 1.0f };
		vertices[1].uv = { 0.0f, 0.0f };
		vertices[2].uv = { 1.0f, 1.0f };
		vertices[3].uv = { 1.0f, 0.0f };


		// 頂点バッファ用リソース設定
		static D3D12_RESOURCE_DESC resDesc;
		resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));   // バッファサイズを設定

		// 頂点バッファ用ヒープ設定
		static D3D12_HEAP_PROPERTIES heapProp;
		heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);  // CPUからUploadできるようにする

		// 頂点データ転送
		static Vertex* vertMap = nullptr;
		g_hr = vertBuff->Map(0, nullptr, (void**)&vertMap);

		// 頂点バッファのマップに失敗したらthrow
		Debugger::CheckHR(g_hr, "頂点バッファのマップの作成に失敗しました");

		// 最初から最後までコピー
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertBuff->Unmap(0, nullptr);

		// 頂点バッファビュー作成
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeof(vertices);
		vbView.StrideInBytes = sizeof(Vertex);

		// 行列の初期化
		static XMMATRIX matrix = XMMatrixIdentity();
		// 行列に座標変換用として設定する
		matrix.r[0].m128_f32[0] = 2.0f / window_width;
		matrix.r[1].m128_f32[1] = -2.0f / window_height;
		matrix.r[0].m128_f32[3] = -1;
		matrix.r[1].m128_f32[3] = 1;

		// 定数バッファデータの初期化
		static ConstantBufferData cbd;
		cbd = { matrix,color };

		// マップ先を示すポインタ
		static ConstantBufferData* mapCBD;
		// マップする
		g_hr = constBuff->Map(0, nullptr, (void**)&mapCBD);

		// 定数バッファのマップに失敗したらthrow
		Debugger::CheckHR(g_hr, "定数バッファのマップに失敗しました");

		// 内容のコピー
		*mapCBD = cbd;

		// 頂点バッファをセット
		DeviceManager::GetCMDList()->IASetVertexBuffers(0, 1, &vbView);

		// インデックスバッファをセット
		DeviceManager::GetCMDList()->IASetIndexBuffer(&ibView);

		// ディスクリプタヒープセット
		ID3D12DescriptorHeap* descriptorHeaps[] = { descriHeap };
		DeviceManager::GetCMDList()->SetDescriptorHeaps(ARRAYSIZE(descriptorHeaps), descriptorHeaps);

		// ヒープの中のハンドル
		static D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;

		// 最初を指す
		heapHandle = descriHeap->GetGPUDescriptorHandleForHeapStart();

		// ハンドルの位置にセット(0はSRVのRootParameter)
		DeviceManager::GetCMDList()->SetGraphicsRootDescriptorTable(0, heapHandle);

		// ディスクリプタのサイズ分進む
		heapHandle.ptr += DeviceManager::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// ハンドルの位置にセット(1はCBVのRootParameter)
		DeviceManager::GetCMDList()->SetGraphicsRootDescriptorTable(1, heapHandle);

		// 描画
		DeviceManager::GetCMDList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite描画で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

ULONGLONG Sprite::AligmentedSize(ULONGLONG size, ULONGLONG alignment) {

	return (size + alignment - 1) & ~(alignment - 1);
}
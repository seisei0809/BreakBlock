#include "AllSetting.h"
#include "Sprite.h"
#include "SpriteObjectManager.h"
#include "DeviceManager.h"
#include "Debugger.h"
#include "AssetManager.h"

// static�����o�̎��̂��`

std::map<std::wstring, ID3D12Resource*> Sprite::texBuffCache;
std::map<std::wstring, TexMetadata> Sprite::metadataCache;
std::map<std::wstring, ScratchImage> Sprite::scratchImgCache;
UINT64 Sprite::fenceValDefault = 0;

Sprite::Sprite(WSTR path, ID3D12Fence* fence, UINT64& fenceVal)
	: transform({ {0, 0, 0}, 0.0f, {1, 1} }),   // �������W
	vbView(),
	ibView(),
	vertBuff(nullptr),
	uploadBuff(nullptr),
	idxBuff(nullptr),
	texBuff(nullptr),
	constBuff(nullptr),
	descriHeap(nullptr),
	color(1.0f, 1.0f, 1.0f, 1.0f),              // ���F
	isActive(true),                             // �A�N�e�B�u���
	imageName(path),
	uploadAllocator(nullptr),
	uploadCmdList(nullptr),
	uploadCmdQueue(nullptr),
	fence(fence),
	fenceVal(fenceVal)
{

	try
	{
		// �����Ƀt�F���X���Ȃ���΃f�t�H���g
		if (!fence) {

			fence = DeviceManager::GetFence(0);
			fenceVal = DeviceManager::GetFenceVal(0);
		}

		// �C���f�b�N�X�f�[�^
		static USHORT indices[] = { 0, 1, 2, 2, 1, 3 };

		// �ŏ��̓C���f�b�N�X�f�[�^�p
		static D3D12_RESOURCE_DESC resDesc;
		resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices));   // ���\�[�X�̐ݒ�
		static D3D12_HEAP_PROPERTIES heapProp;
		heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD); // �q�[�v�̐ݒ�

		// �C���f�b�N�X�o�b�t�@�쐬
		g_hr = DeviceManager::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&idxBuff));

		// �C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���");

		// �C���f�b�N�X�f�[�^�]��
		static USHORT* mappedIdx = nullptr;
		g_hr = idxBuff->Map(0, nullptr, (void**)&mappedIdx);

		// �C���f�b�N�X�o�b�t�@�̃}�b�v�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�C���f�b�N�X�o�b�t�@�̃}�b�v�Ɏ��s���܂���");

		// �ŏ�����Ō�܂ŃR�s�[
		std::copy(std::begin(indices), std::end(indices), mappedIdx);
		idxBuff->Unmap(0, nullptr);

		// �C���f�b�N�X�o�b�t�@�r���[�쐬
		ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeof(indices);

		// ���_�̈ꎞ�ϐ�  
		static Vertex vertices[4];

		// ���_���Z�b�g
		vertices[0] = { {0.0f,1.0f,0.0f}, { 0.0f, 1.0f } };
		vertices[1] = { {0.0f,0.0f,0.0f},{ 0.0f, 0.0f } };
		vertices[2] = { {1.0f,1.0f,0.0f},{ 1.0f, 1.0f } };
		vertices[3] = { {1.0f,0.0f,0.0f},{ 1.0f, 0.0f } };

		// ���_�o�b�t�@�p�ɏ���������
		resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));

		// ���_�o�b�t�@�쐬
		g_hr = DeviceManager::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));

		// ���_�o�b�t�@�̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���");

		// ���_�o�b�t�@�r���[�쐬
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeof(vertices);
		vbView.StrideInBytes = sizeof(Vertex);

		//���^�f�[�^(�摜�̏��)
		TexMetadata metadata = {};
		// �摜����
		ScratchImage scratchImg = {};

		// �L���b�V�����烍�[�h�������ǂ���
		bool loadedFromCache = false;
		// �L���b�V���Ɋ��ɂ��邩�ǂ���
		auto it = texBuffCache.find(imageName);
		if (it != texBuffCache.end()) {

			// �L���b�V���ɂ������̂Ŏg����
			texBuff = it->second;
			metadata = metadataCache[imageName];
			scratchImg = std::move(scratchImgCache[imageName]);
			// ���[�h�������Ƃɂ���
			loadedFromCache = true;
		}

		// �L���b�V�����烍�[�h�����Ȃ�X�L�b�v
		if (!loadedFromCache) {

			// �p�b�N���ꂽ�t�@�C���̃��[�h
			static VECTOR<PackedFile> files = AssetManager::LoadPack();

			// for�J�E���g�p
			INT count = 0;
			// �W�J����������T��
			for (auto& file : files) {

				count++;
				// ���O�ƈ�v������
				if (file.name == imageName) {

					// WIC�摜�Ƃ��ă��[�h����
					g_hr = LoadFromWICMemory(
						file.data.data(),                  // ��������̃f�[�^�擪
						file.data.size(),                  // �T�C�Y
						WIC_FLAGS_NONE,                    // �t���O
						&metadata,                         // ���^�f�[�^�i�[��
						scratchImg                         // �C���[�W�i�[��
					);

					// WIC�摜�̃��[�h�Ɏ��s������throw
					Debugger::CheckHR(g_hr, "WIC�摜�̃��[�h�Ɏ��s���܂���");

					break;
				}

				// �Ō�܂ōs������throw
				if (files.size() == count) {

					throw std::runtime_error(std::string("�摜��������܂���ł���:"));
				}
			}



			// �摜�̒��o�p
			static const Image* img;
			// �摜�̒��o
			img = scratchImg.GetImage(0, 0, 0);

			// �s�N�Z���T�C�Y(B8G8R8A8_UNORM_SRGB��1�s�N�Z��������4�o�C�g�Ȃ̂�)
			static size_t pixelSize = 4;

			// �s���Ƃ̃T�C�Y
			static size_t rowPitch;
			// 256�����s���������̂ŃA���C�����g
			rowPitch = AligmentedSize(img->width * pixelSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

			// �e�N�X�`���o�b�t�@�p�ɂ���
			resDesc.Width = rowPitch * img->height;

			// ���ԃo�b�t�@�̍쐬
			g_hr = DeviceManager::GetDevice()->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadBuff)
			);

			// �A�b�v���[�h�o�b�t�@�̍쐬�Ɏ��s������throw
			Debugger::CheckHR(g_hr, "�A�b�v���[�h�o�b�t�@�̍쐬�Ɏ��s���܂���");

			// �s�N�Z�������A�b�v���[�h�o�b�t�@�ɑ���
			static UINT8* mapforImg = nullptr;
			g_hr = uploadBuff->Map(0, nullptr, (void**)&mapforImg);

			// �A�b�v���[�h�o�b�t�@�̃}�b�v�Ɏ��s������throw
			Debugger::CheckHR(g_hr, "�A�b�v���[�h�o�b�t�@�̃}�b�v�Ɏ��s���܂���");

			// �s�N�Z���f�[�^�̃A�h���X
			static UINT8* srcAddres;
			srcAddres = img->pixels;

			// 1�s�����̂ō����܂�
			for (INT y = 0; y < img->height; y++) {

				// ���̍s�ɍs���R�s�[����
				std::copy_n(srcAddres,
					rowPitch,
					mapforImg);

				// ��s���Ƃ̂��܂����킹��
				srcAddres += img->rowPitch;
				mapforImg += rowPitch;
			}
			uploadBuff->Unmap(0, nullptr);

			// �e�N�X�`���o�b�t�@�p�ɂ���
			heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			resDesc.Format = metadata.format;
			resDesc.Width = (UINT64)metadata.width;
			resDesc.Height = (UINT)metadata.height;
			resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
			resDesc.MipLevels = (UINT16)metadata.mipLevels;
			resDesc.Dimension = (D3D12_RESOURCE_DIMENSION)(metadata.dimension);
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

			// �e�N�X�`���o�b�t�@�̍쐬
			g_hr = DeviceManager::GetDevice()->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // �e�N�X�`���p�w��
				nullptr,
				IID_PPV_ARGS(&texBuff)
			);

			// �R�s�[���ݒ�
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

			// �R�s�[��ݒ�
			dst.pResource = texBuff;
			dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dst.SubresourceIndex = 0;

			// ���\�[�X�o���A��ݒ�(�R�s�[�p)
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				texBuff,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_COPY_DEST
			);

			// �R�}���h�A���P�[�^�쐬
			g_hr = DeviceManager::GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&uploadAllocator));
			Debugger::CheckHR(g_hr, "�A�b�v���[�h�p�R�}���h�A���P�[�^�̍쐬�Ɏ��s");

			// �R�}���h���X�g�쐬
			g_hr = DeviceManager::GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, uploadAllocator, nullptr, IID_PPV_ARGS(&uploadCmdList));
			Debugger::CheckHR(g_hr, "�A�b�v���[�h�p�R�}���h���X�g�̍쐬�Ɏ��s");

			// �R�}���h�L���[�쐬
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			g_hr = DeviceManager::GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&uploadCmdQueue));
			Debugger::CheckHR(g_hr, "�A�b�v���[�h�p�R�}���h�L���[�̍쐬�Ɏ��s");

			// ���Z�b�g
			uploadAllocator->Reset();

			// ���Z�b�g
			uploadCmdList->Reset(uploadAllocator, nullptr);

			// ���\�[�X�o���A���Ƃ�
			uploadCmdList->ResourceBarrier(1, &barrier);

			// �e�N�X�`���o�b�t�@��Upload�p�̓��e���R�s�[����
			uploadCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

			// ����VS�ɑ��鏀��������
			barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				texBuff,
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			);

			// ���\�[�X�o���A���Ƃ�
			uploadCmdList->ResourceBarrier(1, &barrier);

			uploadCmdList->Close();

			// �R�}���h���X�g�̎��s
			ID3D12CommandList* cmdlists[] = { uploadCmdList };
			uploadCmdQueue->ExecuteCommandLists(1, cmdlists);

			uploadCmdQueue->Signal(fence, ++fenceVal);
		}

		// �L���b�V������
		texBuffCache[imageName] = texBuff;
		metadataCache[imageName] = metadata;
		scratchImgCache[imageName] = std::move(scratchImg);

		// �s��̏�����
		static XMMATRIX matrix = XMMatrixIdentity();
		// �s��ɍ��W�ϊ��p�Ƃ��Đݒ肷��
		matrix.r[0].m128_f32[0] = 2.0f / window_width;
		matrix.r[1].m128_f32[1] = -2.0f / window_height;
		matrix.r[0].m128_f32[3] = -1;
		matrix.r[1].m128_f32[3] = 1;

		// �F������
		static XMFLOAT4 color = { 1,1,1,1 };

		// �萔�o�b�t�@�f�[�^�̏�����
		static ConstantBufferData cbd;
		cbd = { matrix,color };

		// �s��p�q�[�v�ݒ�
		static D3D12_HEAP_PROPERTIES heapPropMat = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// �s��p���\�[�X�ݒ�
		static D3D12_RESOURCE_DESC resDescMat = CD3DX12_RESOURCE_DESC::Buffer((sizeof(cbd) + 0xff) & ~0xff);

		// �s��̂��߂̒萔�o�b�t�@�̍쐬
		g_hr = DeviceManager::GetDevice()->CreateCommittedResource(
			&heapPropMat,
			D3D12_HEAP_FLAG_NONE,
			&resDescMat,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff)
		);

		// �萔�o�b�t�@�̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�萔�o�b�t�@�̍쐬�Ɏ��s���܂���");

		// �}�b�v��������|�C���^
		static ConstantBufferData* mapCBD;
		// �}�b�v����
		g_hr = constBuff->Map(0, nullptr, (void**)&mapCBD);

		// �萔�o�b�t�@�̃}�b�v�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�萔�o�b�t�@�̃}�b�v�Ɏ��s���܂���");

		// ���e�̃R�s�[
		*mapCBD = cbd;

		// �f�B�X�N���v�^�q�[�v�쐬(2��)
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 2;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		g_hr = DeviceManager::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriHeap));

		// �f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂���");

		// �f�B�X�N���v�^�q�[�v�̒��̃n���h��
		static D3D12_CPU_DESCRIPTOR_HANDLE handle;

		// �ŏ����w��
		handle = descriHeap->GetCPUDescriptorHandleForHeapStart();

		// SRV�쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = metadata.format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		DeviceManager::GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, handle);

		// CBV�쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
		handle.ptr += DeviceManager::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		DeviceManager::GetDevice()->CreateConstantBufferView(&cbvDesc, handle);

		// Sprite�Ǘ��ɓ����
		SpriteObjectManager::AddSprite(this);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�R���X�g���N�^�ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

Sprite::~Sprite() {

	// uploadBuff�����[�h�����ǂ������邽��
	// �t�F���X����������܂őҋ@
	if (fence && fence->GetCompletedValue() < fenceVal) {
		// �����C�x���g���쐬
		auto event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		// �t�F���X���w�肳�ꂽ�l�ɓ��B������C�x���g�𔭐�������
		fence->SetEventOnCompletion(fenceVal, event);

		if (event != 0) {
			// �C�x���g����������܂őҋ@
			WaitForSingleObject(event, INFINITE);

			// �C�x���g�n���h�������i���\�[�X������j
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
		// �e�N�X�`���o�b�t�@�̉��
		for (auto it = texBuffCache.begin(); it != texBuffCache.end(); ++it) {
			SAFE_RELEASE(it->second);
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�L���b�V���j���ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

void Sprite::Draw() {

	try
	{
		// uploadBuff�����[�h�����ǂ������邽��
		// �܂��Ȃ�`��X�L�b�v
		if (fence && fence->GetCompletedValue() < fenceVal) return;

		// ���_�̈ꎞ�ϐ�  
		static Vertex vertices[4];

		// ��]
		static float angle;
		angle = transform.rotation;

		// ��]�s����v�Z
		static XMMATRIX rotMat;
		rotMat = XMMatrixRotationZ(angle);

		// ���s�ړ��p�̃I�t�Z�b�g
		static XMVECTOR offset;
		offset = XMVectorSet(transform.position.x, transform.position.y, 0.0f, 0.0f);

		// �I�t�Z�b�g�O�̃��[�J�����W
		XMFLOAT2 localPositions[4] =
		{
			{ -transform.scale.x,  transform.scale.y }, // ����
			{ -transform.scale.x, -transform.scale.y }, // ����
			{  transform.scale.x,  transform.scale.y }, // �E��
			{  transform.scale.x, -transform.scale.y }  // �E��
		};

		// ���_���Ƃɉ�]�{�ړ�����
		for (INT i = 0; i < 4; ++i)
		{
			// ���[�J�����W��XMVECTOR��
			static XMVECTOR localPos;
			localPos = XMVectorSet(localPositions[i].x, localPositions[i].y, 0.0f, 1.0f);

			// ��]�s��̓K�p����
			static XMVECTOR rotatedPos;
			rotatedPos = XMVector4Transform(localPos, rotMat);

			// �I�t�Z�b�g�̉��Z
			rotatedPos = XMVectorAdd(rotatedPos, offset);

			// ���ʂ�XMFLOAT3�Ɋi�[�iGPU�p���_�f�[�^�j
			XMStoreFloat3(&vertices[i].position, rotatedPos);
		}

		// UV���Z�b�g
		vertices[0].uv = { 0.0f, 1.0f };
		vertices[1].uv = { 0.0f, 0.0f };
		vertices[2].uv = { 1.0f, 1.0f };
		vertices[3].uv = { 1.0f, 0.0f };


		// ���_�o�b�t�@�p���\�[�X�ݒ�
		static D3D12_RESOURCE_DESC resDesc;
		resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));   // �o�b�t�@�T�C�Y��ݒ�

		// ���_�o�b�t�@�p�q�[�v�ݒ�
		static D3D12_HEAP_PROPERTIES heapProp;
		heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);  // CPU����Upload�ł���悤�ɂ���

		// ���_�f�[�^�]��
		static Vertex* vertMap = nullptr;
		g_hr = vertBuff->Map(0, nullptr, (void**)&vertMap);

		// ���_�o�b�t�@�̃}�b�v�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "���_�o�b�t�@�̃}�b�v�̍쐬�Ɏ��s���܂���");

		// �ŏ�����Ō�܂ŃR�s�[
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertBuff->Unmap(0, nullptr);

		// ���_�o�b�t�@�r���[�쐬
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeof(vertices);
		vbView.StrideInBytes = sizeof(Vertex);

		// �s��̏�����
		static XMMATRIX matrix = XMMatrixIdentity();
		// �s��ɍ��W�ϊ��p�Ƃ��Đݒ肷��
		matrix.r[0].m128_f32[0] = 2.0f / window_width;
		matrix.r[1].m128_f32[1] = -2.0f / window_height;
		matrix.r[0].m128_f32[3] = -1;
		matrix.r[1].m128_f32[3] = 1;

		// �萔�o�b�t�@�f�[�^�̏�����
		static ConstantBufferData cbd;
		cbd = { matrix,color };

		// �}�b�v��������|�C���^
		static ConstantBufferData* mapCBD;
		// �}�b�v����
		g_hr = constBuff->Map(0, nullptr, (void**)&mapCBD);

		// �萔�o�b�t�@�̃}�b�v�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�萔�o�b�t�@�̃}�b�v�Ɏ��s���܂���");

		// ���e�̃R�s�[
		*mapCBD = cbd;

		// ���_�o�b�t�@���Z�b�g
		DeviceManager::GetCMDList()->IASetVertexBuffers(0, 1, &vbView);

		// �C���f�b�N�X�o�b�t�@���Z�b�g
		DeviceManager::GetCMDList()->IASetIndexBuffer(&ibView);

		// �f�B�X�N���v�^�q�[�v�Z�b�g
		ID3D12DescriptorHeap* descriptorHeaps[] = { descriHeap };
		DeviceManager::GetCMDList()->SetDescriptorHeaps(ARRAYSIZE(descriptorHeaps), descriptorHeaps);

		// �q�[�v�̒��̃n���h��
		static D3D12_GPU_DESCRIPTOR_HANDLE heapHandle;

		// �ŏ����w��
		heapHandle = descriHeap->GetGPUDescriptorHandleForHeapStart();

		// �n���h���̈ʒu�ɃZ�b�g(0��SRV��RootParameter)
		DeviceManager::GetCMDList()->SetGraphicsRootDescriptorTable(0, heapHandle);

		// �f�B�X�N���v�^�̃T�C�Y���i��
		heapHandle.ptr += DeviceManager::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// �n���h���̈ʒu�ɃZ�b�g(1��CBV��RootParameter)
		DeviceManager::GetCMDList()->SetGraphicsRootDescriptorTable(1, heapHandle);

		// �`��
		DeviceManager::GetCMDList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("Sprite�`��ňُ�G���[:") + STR(e.what()) + STR(":"));
	}
}

ULONGLONG Sprite::AligmentedSize(ULONGLONG size, ULONGLONG alignment) {

	return (size + alignment - 1) & ~(alignment - 1);
}
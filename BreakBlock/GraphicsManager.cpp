#include "AllSetting.h"
#include "GraphicsManager.h"
#include "Debugger.h"
#include "DeviceManager.h"
#include "SpriteObjectManager.h"

// static�����o�̎��̂��`

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
		// �V�F�[�_�[�o�C�i���i�[�p
		_vsBlob = nullptr;
		_psBlob = nullptr;
		_errorBlob = nullptr;

		// ���΃p�X����R���p�C���ς�VS�t�@�C����ǂݍ���
		g_hr = D3DReadFileToBlob(L"Shaders/VertexShader.cso", &_vsBlob);

		// VS�t�@�C���̓ǂݍ��݂Ɏ��s������throw
		Debugger::CheckHR(g_hr, "VS�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");

		// ���΃p�X����R���p�C���ς�PS�t�@�C����ǂݍ���
		g_hr = D3DReadFileToBlob(L"Shaders/PixelShader.cso", &_psBlob);

		// PS�t�@�C���̓ǂݍ��݂Ɏ��s������throw
		Debugger::CheckHR(g_hr, "PS�t�@�C���̓ǂݍ��݂Ɏ��s���܂���");

		// ���_���C�A�E�g�̒�`
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// �p�C�v���C���ݒ�\���̂̏�����
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
		gpipeline.pRootSignature = nullptr; // ��Őݒ�

		// ���_�V�F�[�_�̃o�C�i���ݒ�
		gpipeline.VS.pShaderBytecode = _vsBlob->GetBufferPointer();
		gpipeline.VS.BytecodeLength = _vsBlob->GetBufferSize();

		// �s�N�Z���V�F�[�_�̃o�C�i���ݒ�
		gpipeline.PS.pShaderBytecode = _psBlob->GetBufferPointer();
		gpipeline.PS.BytecodeLength = _psBlob->GetBufferSize();

		// �T���v���}�X�N�̐ݒ�i�S�r�b�gON�j
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ���X�^���C�U�X�e�[�g�̐ݒ�
		gpipeline.RasterizerState.MultisampleEnable = false;               // �}���`�T���v������
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;         // �J�����O����
		gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;        // �h��Ԃ��`��
		gpipeline.RasterizerState.DepthClipEnable = true;                  // �[�x�N���b�s���O�L��

		// �u�����h�X�e�[�g�̐ݒ�
		gpipeline.BlendState.AlphaToCoverageEnable = false;                // �A���t�@�J�o���b�W����
		gpipeline.BlendState.IndependentBlendEnable = false;               // �Ɨ��u�����h����

		// �e�����_�[�^�[�Q�b�g�p�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		// �u�����h�X�e�[�g�̐ݒ�
		renderTargetBlendDesc.BlendEnable = true;                                   // �u�����h��L����
		renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;                     // �\�[�X�̃A���t�@�l
		renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;                // �t�A���t�@�l
		renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;                         // ���Z�u�����h
		renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;                      // �A���t�@�l�̃\�[�X
		renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;                    // �A���t�@�l�̃f�X�e�B�l�[�V����
		renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;                    // �A���t�@�l�̉��Z
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA���ׂĕ`��

		// �ݒ��K�p
		gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

		// ���_���C�A�E�g�̐ݒ�
		gpipeline.InputLayout.pInputElementDescs = inputLayout;
		gpipeline.InputLayout.NumElements = ARRAYSIZE(inputLayout);

		// �C���f�b�N�X�o�b�t�@�̃J�b�g�l�ݒ�i�����j
		gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

		// �g�|���W�[�̎�ށi�O�p�`���X�g�j
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		gpipeline.NumRenderTargets = 1;
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		// �T���v�����O�̐ݒ�
		gpipeline.SampleDesc.Count = 1;
		gpipeline.SampleDesc.Quality = 0;

		D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};

		descTblRange[0].NumDescriptors = 1;                            // �e�N�X�`���͈��
		descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;   // ��ʂ̓e�N�X�`��
		descTblRange[0].BaseShaderRegister = 0;                        // 0�ԃX���b�g����
		descTblRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		descTblRange[1].NumDescriptors = 1;                            // �萔�͈��
		descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;   // ��ʂ͒萔
		descTblRange[1].BaseShaderRegister = 0;                        // 0�ԃX���b�g����
		descTblRange[1].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER rootparam[2] = {};

		rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// �S�ẴV�F�[�_�[���猩����
		rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];      // �f�B�X�N���v�^�����W�̃A�h���X
		rootparam[0].DescriptorTable.NumDescriptorRanges = 1;                   // �f�B�X�N���v�^�����W��
		rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// �S�ẴV�F�[�_�[���猩����
		rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];      // �f�B�X�N���v�^�����W�̃A�h���X
		rootparam[1].DescriptorTable.NumDescriptorRanges = 1;                   // �f�B�X�N���v�^�����W��

		D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

		samplerDesc.ShaderRegister = 0;                             // �����I�ɃT���v���[��0�X���b�g�ɑ���
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;     // �������̌J��Ԃ�
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;     // �c�����̌J��Ԃ�
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;     // �������̌J��Ԃ�
		samplerDesc.BorderColor =
			D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;            // �{�[�_�[�͍�
		//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;       // ���`�⊮
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;        // �⊮���Ȃ�(�j�A���X�g���C�o�[�@)
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                     // �~�b�v�}�b�v�ő�l
		samplerDesc.MinLOD = 0.0f;                                  // �~�b�v�}�b�v�ŏ��l
		samplerDesc.ShaderVisibility =
			D3D12_SHADER_VISIBILITY_PIXEL;                          // �s�N�Z���V�F�[�_�[���猩����
		samplerDesc.ShaderRegister = 0;                             // �f�t�H���g�ŃX���b�g0
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;   // ���T���v�����O���Ȃ�

		// ���[�g�V�O�l�`���̒�`
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootparam; // ���[�g�p�����[�^�̐擪�A�h���X
		rootSignatureDesc.NumParameters = 2;        // ���[�g�p�����[�^��
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ���[�g�V�O�l�`���̃V���A���C�Y
		ID3DBlob* rootSignatureBlob = nullptr;
		g_hr = D3D12SerializeRootSignature(
			&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &_errorBlob);

		// ���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���");

		// ���[�g�V�O�l�`���쐬
		_rootsignature = nullptr;
		g_hr = DeviceManager::GetDevice()->CreateRootSignature(
			0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&_rootsignature));

		// ���[�g�V�O�l�`���̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���");

		// �g���I������o�C�i�����
		rootSignatureBlob->Release();

		// �p�C�v���C���Ƀ��[�g�V�O�l�`����ݒ�
		gpipeline.pRootSignature = _rootsignature;

		// �r���[�|�[�g�̐ݒ�
		D3D12_VIEWPORT viewport = {};
		viewport.Width = (FLOAT)window_width;
		viewport.Height = (FLOAT)window_height;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		_viewport = viewport;

		// �V�U�[��`�̐ݒ�
		D3D12_RECT scissorrect = {};
		scissorrect.top = 0;
		scissorrect.left = 0;
		scissorrect.right = scissorrect.left + window_width;
		scissorrect.bottom = scissorrect.top + window_height;
		_scissorrect = scissorrect;

		// �p�C�v���C���X�e�[�g�쐬
		_pipelinestate = nullptr;
		g_hr = DeviceManager::GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&_pipelinestate));

		// �p�C�v���C���X�e�[�g�̍쐬�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("�O���t�B�b�N�X�Ǘ����������Ɉُ�G���[:") + e.what() + STR(":"));
	}
}

void GraphicsManager::Render() {

	try
	{
		// �R�}���h�A���P�[�^�̃��Z�b�g�i�V�����R�}���h���L�^�ł���悤�ɂ���j
		g_hr = DeviceManager::GetCMDAllocator()->Reset();

		// �R�}���h�A���P�[�^�̃��Z�b�g�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�R�}���h�A���P�[�^�̃��Z�b�g�Ɏ��s���܂���");

		// �R�}���h���X�g�̃��Z�b�g�i�V�����R�}���h��ǉ����鏀���j
		g_hr = DeviceManager::GetCMDList()->Reset(DeviceManager::GetCMDAllocator(), nullptr);

		// �R�}���h���X�g�̃��Z�b�g�Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���");

		// �p�C�v���C���X�e�[�g�̐ݒ�
		DeviceManager::GetCMDList()->SetPipelineState(_pipelinestate);

		// ���[�g�V�O�l�`����ݒ�
		DeviceManager::GetCMDList()->SetGraphicsRootSignature(_rootsignature);

		// ���݂̃o�b�N�o�b�t�@�̃C���f�b�N�X���擾
		auto bbIdx = DeviceManager::GetSwapChain()->GetCurrentBackBufferIndex();

		// ���\�[�X�o���A��ݒ肵�āA���\�[�X�̏�ԑJ�ڂ��s������
		D3D12_RESOURCE_BARRIER barrierDesc = {};

		// �J�ڂ̎�ނƂ��ă��\�[�X�o���A�̃^�C�v��ݒ�
		barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION; // �J�ڃ^�C�v
		barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; // ���Ƀt���O�͎w�肵�Ȃ�
		barrierDesc.Transition.pResource = DeviceManager::GetBackBuffer(bbIdx); // �J�ڑΏۂ̃��\�[�X�i�o�b�N�o�b�t�@�j
		barrierDesc.Transition.Subresource = 0; // �T�u���\�[�X�͑S��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �J�ڑO�̏�ԁiPRESENT�j
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �J�ڌ�̏�ԁiRENDER_TARGET�j
		// ���\�[�X�o���A���R�}���h���X�g�ɒǉ�
		DeviceManager::GetCMDList()->ResourceBarrier(1, &barrierDesc);

		// RTV�q�[�v����CPU�p�̃f�B�X�N���v�^�n���h�����擾
		auto rtvh = DeviceManager::GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

		// �o�b�N�o�b�t�@�ɑΉ�����RTV�̃f�B�X�N���v�^�ʒu�ɃI�t�Z�b�g��K�p
		rtvh.ptr += (ULONGLONG)bbIdx * DeviceManager::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// �����_�[�^�[�Q�b�g��ݒ�
		DeviceManager::GetCMDList()->OMSetRenderTargets(1, &rtvh, false, nullptr);

		// �����_�[�^�[�Q�b�g�r���[���N���A�i�w�i�F��h��j
		DeviceManager::GetCMDList()->ClearRenderTargetView(rtvh, backColor, 0, nullptr);

		// �r���[�|�[�g��ݒ�
		DeviceManager::GetCMDList()->RSSetViewports(1, &_viewport);

		// �X�J�V��`��ݒ�
		DeviceManager::GetCMDList()->RSSetScissorRects(1, &_scissorrect);

		// �g���C�A���O�����X�g�Ƃ��Ă̍\���Ƃ���
		DeviceManager::GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Sprite�̕`��������ōs��
		SpriteObjectManager::Draw();

		// �`���̃��\�[�X��ԑJ�ڂ��ēx�ݒ�iRENDER_TARGET �� PRESENT�j
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �J�ڑO�̏�ԁiRENDER_TARGET�j
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �J�ڌ�̏�ԁiPRESENT�j
		// �ēx���\�[�X�o���A���R�}���h���X�g�ɒǉ�
		DeviceManager::GetCMDList()->ResourceBarrier(1, &barrierDesc);

		// �R�}���h���X�g����āA�R�}���h�̋L�^���I��
		g_hr = DeviceManager::GetCMDList()->Close();

		// �R�}���h���X�g�����̂Ɏ��s������throw
		Debugger::CheckHR(g_hr, "�R�}���h���X�g�����̂Ɏ��s���܂���");

		// �R�}���h���X�g�����s���邽�߂Ƀ��X�g���w��
		ID3D12CommandList* cmdlists[] = { DeviceManager::GetCMDList() };

		// �R�}���h�L���[�ɃR�}���h���X�g��ǉ����Ď��s
		DeviceManager::GetCMDQueue()->ExecuteCommandLists(ARRAYSIZE(cmdlists), cmdlists);

		// �O�񂱂̃o�b�t�@���g��������GPU������҂�
		if (DeviceManager::GetFence(bbIdx)->GetCompletedValue() < DeviceManager::GetFenceVal(bbIdx)) {
			HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			DeviceManager::GetFence(bbIdx)->SetEventOnCompletion(DeviceManager::GetFenceVal(bbIdx), event);
			if (event != 0) {
				WaitForSingleObject(event, INFINITE);
				CloseHandle(event);
			}
		}

		// �t�F���X�ɃV�O�i���𑗂�(GPU�ɏ����������������Ƃ�m�点��)
		DeviceManager::GetCMDQueue()->Signal(DeviceManager::GetFence(bbIdx), ++DeviceManager::GetFenceVal(bbIdx));

		// �X���b�v�`�F�C���̔���(�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@��؂�ւ���)
		// �������Sleep���Ă�̂Ő��������Ȃ�
		g_hr = DeviceManager::GetSwapChain()->Present(0, 0);
	}
	catch (const std::exception& e)
	{
		// ����Ȃ�G���[�o��
		if (_errorBlob) {

			OutputDebugStringA((char*)_errorBlob->GetBufferPointer());
		}
		throw std::runtime_error(STR("�O���t�B�b�N�X�Ǘ��`�撆�Ɉُ�G���[:") + e.what() + STR(":"));
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
		throw std::runtime_error(STR("�O���t�B�b�N�X�Ǘ������[�X���Ɉُ�G���[:") + STR(e.what()) + STR(":"));
	}
}
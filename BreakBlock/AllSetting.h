#pragma once
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <debugapi.h>
#include <dxgidebug.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <d3dx12.h>
#include <xaudio2.h>
#include "resource.h"

using namespace DirectX;

template<typename T>
using VECTOR = std::vector<T>;
using WSTR = std::wstring;
using STR = std::string;

// �|�C���^���L���Ȃ�Release���Ăяo���Anullptr�ɐݒ�
#define SAFE_RELEASE(p) if(p){ p->Release();p = nullptr;}
// �|�C���^���L���Ȃ�delete���Ăяo���Anullptr�ɐݒ�
#define SAFE_DELETE(p) if (p) {delete p;p = nullptr;}
// �|�C���^���L���Ȃ珊�L���̈ړ�(p1->p2)
#define SAFE_MOVE(p1,p2)if (p1) {if(p2){p2->Release();}p2=p1;p1->AddRef();p1->Release();p1 = nullptr;}

extern HRESULT g_hr;			// ���������������s������
extern LONG window_width;		// �E�B���h�E�̕�
extern LONG window_height;		// �E�B���h�E�̍���
extern LPCWSTR g_appName;		// �Q�[���̖��O
extern HWND g_hwnd;				// �Q�[����`�悷��E�B���h�E�̃n���h��
extern FLOAT fps;				// �Œ�FPS
extern FLOAT frameDurationS;	// 1�t���[���̎���(s)
extern FLOAT backColor[4];		// �w�i�F

// �E�B���h�E�v���V�[�W��
LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
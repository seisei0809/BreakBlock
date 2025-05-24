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

// ポインタが有効ならReleaseを呼び出し、nullptrに設定
#define SAFE_RELEASE(p) if(p){ p->Release();p = nullptr;}
// ポインタが有効ならdeleteを呼び出し、nullptrに設定
#define SAFE_DELETE(p) if (p) {delete p;p = nullptr;}
// ポインタが有効なら所有権の移動(p1->p2)
#define SAFE_MOVE(p1,p2)if (p1) {if(p2){p2->Release();}p2=p1;p1->AddRef();p1->Release();p1 = nullptr;}

extern HRESULT g_hr;			// 処理が成功か失敗か見る
extern LONG window_width;		// ウィンドウの幅
extern LONG window_height;		// ウィンドウの高さ
extern LPCWSTR g_appName;		// ゲームの名前
extern HWND g_hwnd;				// ゲームを描画するウィンドウのハンドル
extern FLOAT fps;				// 固定FPS
extern FLOAT frameDurationS;	// 1フレームの時間(s)
extern FLOAT backColor[4];		// 背景色

// ウィンドウプロシージャ
LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
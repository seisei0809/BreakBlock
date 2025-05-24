#include "AllSetting.h"

// グローバル変数定義

HRESULT g_hr = S_OK;
LONG window_width = 800;
LONG window_height = 600;
LPCWSTR g_appName = L"せいせいのブロック崩し";
HWND g_hwnd;
FLOAT fps = 120;
FLOAT frameDurationS = 1.0f / fps;
FLOAT backColor[4] = { 0.0f,0.0f,0.0f,1.0f };

// ウィンドウプロシージャ
LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg)
	{
	case WM_DESTROY:
		// アプリ終了
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
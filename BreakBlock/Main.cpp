#include "AllSetting.h"
#include "Game.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,            // 引数 hInstance は呼び出し元から入力される
	_In_opt_ HINSTANCE hPrevInstance,    // 引数 hPrevInstance は NULL でも問題ない（オプション）
	_In_ LPSTR lpCmdLine,                // 引数 lpCmdLine は呼び出し元から入力される
	_In_ int nCmdShow                    // 引数 nCmdShow は呼び出し元から入力される
) {
	
	try
	{
		// ゲームを開始する
		Game::GameStart();

		// 正常終了
		return 0;
	}
	catch (const std::exception&)
	{
		// 異常終了
		return 1;
	}
}
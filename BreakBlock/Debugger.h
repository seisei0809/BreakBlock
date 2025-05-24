#pragma once
#include "AllSetting.h"

// デバッグ関係
class Debugger
{
public:

	/// <summary>
	/// DirectX関係のデバッグ出力をする
	/// </summary>
	static void EnableDebugLayer();

	/// <summary>
	/// 処理の成功失敗をMessageBoxで出す
	/// </summary>
	/// <param name="">成功か失敗か判断したいHRESULT</param>
	/// <param name="">キャプションにどのように出すか</param>
	static void ShowHRESULT(HRESULT, const WSTR & = L"HRESULT");

	/// <summary>
	/// 処理が成功しているかどうか
	/// </summary>
	/// <param name="">成功か失敗か見る</param>
	/// <param name="">失敗した時に出すメッセージを入れる</param>
	static void CheckHR(HRESULT, STR);
};


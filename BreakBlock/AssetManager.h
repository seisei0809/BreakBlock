#pragma once
#include "AllSetting.h"

// パックされたデータの情報
struct PackedFile {

	WSTR name;	        // ファイルの名前
	VECTOR<UINT8> data;	// データそのもの
};

// アセット管理
class AssetManager {
public:

	/// <summary>
	/// パックされたものをロードして返す
	/// </summary>
	/// <returns>パックされたものの配列</returns>
	static VECTOR<PackedFile> LoadPack();

private:

	// パックしたファイルのパス
	static WSTR _packPath;
};


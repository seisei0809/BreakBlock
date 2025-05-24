#include "AllSetting.h"
#include "AssetManager.h"

// staticメンバの実体を定義する

WSTR AssetManager::_packPath = L"SeiseiPack.asset";

VECTOR<PackedFile> AssetManager::LoadPack() {

	try
	{
		VECTOR<PackedFile> files;  // 復元したファイルを格納するベクター

		// バイナリモードで入力ファイルを開く
		std::ifstream inFile(_packPath, std::ios::binary);

		if (!inFile) return files;  // 開けなければ空のまま終了

		while (true) {

			UINT nameLength = 0;

			// ファイル名の長さを読み込む
			inFile.read((char*)&nameLength, sizeof(nameLength));

			if (inFile.eof() || inFile.fail()) break;  // 読み込み失敗や終端なら終了

			WSTR fileName;
			fileName.resize(nameLength);  // ファイル名の長さ分バッファを確保

			// ファイル名（wchar_t配列）を読み込む
			inFile.read((char*)&fileName[0], nameLength * sizeof(wchar_t));
			if (inFile.fail()) break;  // 失敗したら終了

			UINT dataSize = 0;

			// ファイルデータのサイズ（バイト数）を読み込む（4バイト）
			inFile.read((char*)&dataSize, sizeof(dataSize));
			if (inFile.fail()) break;

			// データ本体を読み込むためのバッファを確保
			VECTOR<UINT8> data(dataSize);

			if (dataSize > 0) {
				// 実際のデータを読み込む
				inFile.read((char*)&data[0], dataSize);
				if (inFile.fail()) break;
			}

			// 読み込んだファイル名とデータを構造体にまとめて追加
			files.push_back({ std::move(fileName), std::move(data) });
		}

		return files;  // 全ファイルを格納したベクターを返す
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("アセットファイルロード中に異常エラー:") + STR(e.what()) + STR(":"));
	}
}
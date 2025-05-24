#include "../AllSetting.h"
#include "AudioPlayer.h"
#include "../Debugger.h"
#include "../AssetManager.h"

AudioPlayer::AudioPlayer(WSTR path)
	:
	xaudio2(nullptr),
	masterVoice(nullptr),
	waveFormat(WAVEFORMATEX()) {

	try
	{
		// XAudio2デバイスの作成
		g_hr = XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
		Debugger::CheckHR(g_hr, "XAudio2の初期化に失敗");

		// マスターボイスの作成
		g_hr = xaudio2->CreateMasteringVoice(&masterVoice);
		Debugger::CheckHR(g_hr, "マスターボイスの作成に失敗");

		// マスターボイスの音量を最大に設定
		g_hr = masterVoice->SetVolume(1.0f);
		Debugger::CheckHR(g_hr, "マスターボイスの音量設定に失敗");

		// パックファイルからWAVをロード
		auto packedFiles = AssetManager::LoadPack();
		for (auto& file : packedFiles) {

			// 指定されたパスと一致するファイルを探す
			if (file.name == path) {

				// 一致したらWAVデータをメモリから読み込む
				LoadWavFromMemory(file.data);
			}
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("音管理コンストラクタで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

AudioPlayer::~AudioPlayer() {

	ClearAll();

	if (masterVoice) {

		// マスターボイス削除
		masterVoice->DestroyVoice();
		masterVoice = nullptr;
	}

	if (xaudio2) SAFE_RELEASE(xaudio2);
}

void AudioPlayer::LoadWavFromMemory(const std::vector<UINT8> data) {

	try
	{
		// waveファイルは最低44バイト以上ある
		if (data.size() < 44) {
			throw std::runtime_error("WAVデータが小さすぎる (44byte未満)");
		}

		const CHAR* ptr = (CHAR*)(data.data()); // 先頭ポインタ取得
		const CHAR* end = ptr + data.size();    // 終端ポインタ

		// RIFF/WAVEヘッダの確認
		if (std::memcmp(ptr, "RIFF", 4) != 0 || std::memcmp(ptr + 8, "WAVE", 4) != 0) {
			throw std::runtime_error("無効なWAVヘッダ");
		}

		// チャンクの先頭（ヘッダ分スキップ）
		const CHAR* p = ptr + 12;

		bool fmtFound = false;
		bool dataFound = false;

		// チャンクを順に走査
		while (p + 8 <= end) {

			const CHAR* chunkId = p;                // チャンク名（"fmt "など）
			UINT32 chunkSize = *(UINT32*)(p + 4);   // チャンクサイズ
			const CHAR* chunkData = p + 8;          // チャンクのデータ部

			// チャンクサイズ異常チェック
			if (chunkData + chunkSize > end) {
				throw std::runtime_error("チャンクサイズ異常");
			}

			// fmtチャンクの場合（音声フォーマット）
			if (std::memcmp(chunkId, "fmt ", 4) == 0) {

				// フォーマットID取得
				USHORT formatTag = *(USHORT*)(chunkData);

				if (formatTag != WAVE_FORMAT_PCM) {
					throw std::runtime_error("非PCM形式のWAV。対応してません");
				}

				// 16バイトの場合標準PCMなので拡張フォーマットではない
				if (chunkSize == 16) {

					// 16バイトのみフォーマット構造体にコピー
					std::memcpy(&waveFormat, chunkData, 16);
					// 拡張サイズなしに設定
					waveFormat.cbSize = 0;
				}
				else if (chunkSize >= sizeof(WAVEFORMATEX)) {

					// サイズ分フォーマット構造体にコピー
					std::memcpy(&waveFormat, chunkData, sizeof(WAVEFORMATEX));
				}
				else {
					throw std::runtime_error("fmtチャンクは16バイト以上必要です");
				}

				// 見つけたことにする
				fmtFound = true;
			}
			// dataチャンクの場合（実際の音声データ）
			else if (std::memcmp(chunkId, "data", 4) == 0) {
				// データサイズの調整
				audioData.resize(chunkSize);
				// サイズ分データをコピー
				std::memcpy(audioData.data(), chunkData, chunkSize);

				// 見つけたことにする
				dataFound = true;
			}

			// 次のチャンクへ
			p += 8 + chunkSize;
		}

		if (!fmtFound) {
			throw std::runtime_error("fmtチャンクが見つからない");
		}
		if (!dataFound) {
			throw std::runtime_error("dataチャンクが見つからない");
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("音管理ロードで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void AudioPlayer::Play(bool loop) {

	try
	{
		// 初期化されていなければ再生しない
		if (!xaudio2 || audioData.empty()) return;

		// 音バッファの確保
		IXAudio2SourceVoice* newVoice = nullptr;
		g_hr = xaudio2->CreateSourceVoice(&newVoice, &waveFormat);
		Debugger::CheckHR(g_hr, "SourceVoiceの作成に失敗");

		XAUDIO2_BUFFER buffer = {};
		buffer.AudioBytes = (UINT32)(audioData.size());         // 音声データサイズ
		buffer.pAudioData = audioData.data();                   // 音声データポインタ
		buffer.Flags = loop ? 0 : XAUDIO2_END_OF_STREAM;        // ループ再生設定
		buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;    // ループ回数設定(ループなら無限、違うなら0)

		// バッファを送信
		g_hr = newVoice->SubmitSourceBuffer(&buffer);
		if (FAILED(g_hr)) {
			newVoice->DestroyVoice();
			throw std::runtime_error("SourceBufferの送信に失敗");
		}

		// 再生開始
		g_hr = newVoice->Start(0);
		if (FAILED(g_hr)) {
			newVoice->DestroyVoice();
			throw std::runtime_error("音声再生開始に失敗");
		}

		// 確保した音バッファをリストに保存
		activeVoices.push_back(newVoice);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("音管理再生で異常エラー:") + STR(e.what()) + STR(":"));
	}
}

void AudioPlayer::ClearAll() {

	try
	{
		// 全て止めて全て破棄
		for (auto& voice : activeVoices) {
			if (voice) {
				voice->Stop(0);
				voice->DestroyVoice();
			}
		}
		activeVoices.clear();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(STR("音管理クリーンで異常エラー:") + STR(e.what()) + STR(":"));
	}
}

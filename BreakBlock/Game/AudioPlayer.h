#pragma once
#include "../AllSetting.h"

// 音再生
class AudioPlayer {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="path">パス名</param>
	AudioPlayer(WSTR);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioPlayer();

	/// <summary>
	/// メモリからwaveファイルをロードする
	/// </summary>
	/// <param name="data">データ</param>
	void LoadWavFromMemory(const std::vector<UINT8>);

	/// <summary>
	/// 再生する
	/// </summary>
	/// <param name="loop">ループするかどうか</param>
	void Play(bool = false);

	/// <summary>
	/// 重複して流れてるものや流し済みのものをまとめて綺麗にする
	/// </summary>
	void ClearAll();

private:

	// XAudionのデバイス(本体)
	IXAudio2* xaudio2;

	// マスターボイス(スピーカーとつながる最終地点)
	IXAudio2MasteringVoice* masterVoice;

	// 再生のために必要な音データ
	std::vector<UINT8> audioData;

	// waveファイルのフォーマット
	WAVEFORMATEX waveFormat;

	// 音バッファの管理配列
	std::vector<IXAudio2SourceVoice*> activeVoices;
};
#pragma once
#include <map>
#include <string>
#include <list>
#include <memory>
/// <summary>
/// 音(BGM,SE)を管理するシングルトンクラス
/// </summary>
class SoundManager final
{
private:
	//シングルトンの準備
	SoundManager() = default;
	~SoundManager() = default;
	//コピー禁止
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator = (const SoundManager&) = delete;
	//ムーブ禁止
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator = (SoundManager&&) = delete;
public:
	//インスタンスを取得
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}
	//初期化
	void Init();
	//更新
	void Update();
	//再生
	void PlayBGM(std::string name);
	//停止
	void Stop();
	//終了
	void End();
private:
	//SEの音量
	float m_seVolume;
	//BGMの音量
	float m_bgmVolume;
	//ボイスの音量
	float m_voiceVolume;
	//マスターの音量
	float m_masterVolume;
	//ハンドルの配列
	std::map<std::string, int> m_soundHandles;
	//再生中ハンドル(SEとVCのみ)
	std::list<int> m_playSEVCHandles;
	//BGM
	int m_playBGMHandle;
};

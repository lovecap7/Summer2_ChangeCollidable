#pragma once
#include <map>
#include <string>
#include <list>
#include <memory>
class SoundBase;
class BGM;
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
	//登録
	void Entry(std::shared_ptr<SoundBase> sound);
	//登録解除
	void Exit(std::shared_ptr<SoundBase> sound);
	//初期化
	void Init();
	//更新
	void Update();
	//終了
	void End();
	//BGM再生
	void PlayBGM(std::string name);
	//BGM停止
	void StopBGM();
	//1度だけ再生する 
	void PlayOnceSE(std::string name);
	//ループ再生する
	void PlayLoopSE(std::string name);
	//1度だけ再生する 
	void PlayVC(std::string name);
	//全ての再生を開始
	void AllPlay();
	//全ての再生を止める
	void AllStop();
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
	//サウンド
	std::list<std::shared_ptr<SoundBase>> m_sounds;
	//BGMは常に一つなのでここで扱う
	std::shared_ptr<BGM> m_bgm;
};

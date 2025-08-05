#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//デフォルトのサウンドの倍率
	constexpr int kDefaultVolume = 127;
}

void SoundManager::Init()
{
	//音量の設定
	m_seVolume = kDefaultVolume;
	m_bgmVolume = kDefaultVolume;
	m_voiceVolume = kDefaultVolume;
	m_masterVolume = kDefaultVolume;
	//音のハンドルロード
	m_soundHandles["TitleBGM"] = LoadSoundMem(L"Data/Sound/BGM/Title.mp3");
	m_soundHandles["SelectStageBGM"] = LoadSoundMem(L"Data/Sound/BGM/SelectStage.mp3");
	m_soundHandles["Stage1BGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage1.mp3");
	m_soundHandles["Stage2BGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage2.mp3");
	m_soundHandles["Stage1_BossBGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage1_Boss.mp3");
	m_soundHandles["Stage2_BossBGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage2_Boss.mp3");
	m_soundHandles["Stage3_BossBGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage3_Boss.mp3");
	//ロードに成功したかチェック
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
	//再生用ハンドル
	m_playBGMHandle = -1;
	m_playSEVCHandles = {};
}
void SoundManager::Update()
{
	//再生が終了したハンドルを消す
	auto remIt = std::remove_if(
		m_playSEVCHandles.begin(), //要素の頭
		m_playSEVCHandles.end(), //要素の終わり
		//ラムダ式(無名関数)
		[](int& handle)->bool {
			//再生してないなら
			auto isPlayEnd = !CheckSoundMem(handle);
			if (isPlayEnd)
			{
				//削除
				DeleteSoundMem(handle);
			}
			return isPlayEnd;
		});
	//配列からも削除
	m_playSEVCHandles.erase(remIt, m_playSEVCHandles.end());
}

void SoundManager::PlayBGM(std::string name)
{
	//再生中のハンドルがあるなら
	if (m_playBGMHandle >= 0)
	{
		//再生を止める
		StopSoundMem(m_playBGMHandle);
		//削除
		DeleteSoundMem(m_playBGMHandle);
	}
	//BGMの再生
	m_playBGMHandle = DuplicateSoundMem(m_soundHandles[name]);
	PlaySoundMem(m_playBGMHandle, DX_PLAYTYPE_LOOP, true);
	ChangeVolumeSoundMem(m_bgmVolume, m_playBGMHandle);
}

void SoundManager::Stop()
{
	StopSoundFile();
}

void SoundManager::End()
{
	//削除
	for (const auto& [key, value] : m_soundHandles) {
		DeleteSoundMem(value);
	}
	m_soundHandles.clear();
	InitSoundMem();
}
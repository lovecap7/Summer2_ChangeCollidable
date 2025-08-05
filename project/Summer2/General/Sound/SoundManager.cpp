#include "SoundManager.h"
#include "SoundBase.h"
#include "BGM.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//デフォルトのサウンドの倍率
	constexpr int kDefaultVolume = 127;
}

void SoundManager::Entry(std::shared_ptr<SoundBase> sound)
{
	//すでに登録されているならしない
	auto it = std::find(m_sounds.begin(), m_sounds.end(), sound);
	if (it != m_sounds.end())return;
	//登録
	m_sounds.emplace_back(sound);
}

void SoundManager::Exit(std::shared_ptr<SoundBase> sound)
{
	//登録されていないならしない
	auto it = std::find(m_sounds.begin(), m_sounds.end(), sound);
	if (it == m_sounds.end())return;
	//解除
	m_sounds.remove(sound);
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
}
void SoundManager::Update()
{
	//再生が終了したハンドルを消す

}

void SoundManager::End()
{
	//削除
	for (const auto& [key, value] : m_soundHandles) {
		DeleteSoundMem(value);
	}
	for (auto& sound : m_sounds) {
		sound->Delete();
	}
	m_sounds.clear();
	m_soundHandles.clear();
	InitSoundMem();
}

void SoundManager::PlayBGM(std::string name)
{
	//すでに実体があるとき終了処理をする
	if (m_bgm)
	{
		m_bgm->End();
	}
	m_bgm = std::make_shared<BGM>(DuplicateSoundMem(m_soundHandles[name]),m_bgmVolume);
	m_bgm->Init();
	m_bgm->Play();
}

void SoundManager::StopBGM()
{
	m_bgm->Stop();
}

void SoundManager::PlayOnceSE(std::string name)
{
}

void SoundManager::PlayLoopSE(std::string name)
{
}

void SoundManager::PlayVC(std::string name)
{
}

void SoundManager::AllPlay()
{
}

void SoundManager::AllStop()
{
}

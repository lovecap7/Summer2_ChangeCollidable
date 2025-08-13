#include "SoundManager.h"
#include "SoundBase.h"
#include "BGM.h"
#include "SE.h"
#include "Voice.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//デフォルトのサウンドの倍率
	constexpr int kDefaultVolume = 127;
	//補正倍率
	constexpr float kCorrectionRate = 1.2f;
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
	m_seVolume = kDefaultVolume * kCorrectionRate;
	m_bgmVolume = kDefaultVolume;
	m_voiceVolume = kDefaultVolume * kCorrectionRate;
	m_masterVolume = kDefaultVolume;
	//音のハンドルロード
	//BGM
	m_soundHandles["TitleBGM"] = LoadSoundMem(L"Data/Sound/BGM/Title.mp3");
	m_soundHandles["SelectStageBGM"] = LoadSoundMem(L"Data/Sound/BGM/SelectStage.mp3");
	m_soundHandles["Stage1BGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage1.mp3");
	m_soundHandles["Stage2BGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage2.mp3");
	m_soundHandles["Stage1_BossBGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage1_Boss.mp3");
	m_soundHandles["Stage2_BossBGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage2_Boss.mp3");
	m_soundHandles["Stage3_BossBGM"] = LoadSoundMem(L"Data/Sound/BGM/Stage3_Boss.mp3");
	m_soundHandles["WinBGM"] = LoadSoundMem(L"Data/Sound/BGM/Win.mp3");
	m_soundHandles["ResultBGM"] = LoadSoundMem(L"Data/Sound/BGM/Result.mp3");
	//SE
	m_soundHandles["Decide"] = LoadSoundMem(L"Data/Sound/SE/Decide.mp3");
	m_soundHandles["Select"] = LoadSoundMem(L"Data/Sound/SE/Select.mp3");
	m_soundHandles["Cancel"] = LoadSoundMem(L"Data/Sound/SE/Cancel.mp3");
	m_soundHandles["GetItem"] = LoadSoundMem(L"Data/Sound/SE/GetItem.wav");
	m_soundHandles["Wind"] = LoadSoundMem(L"Data/Sound/SE/Wind.mp3");
	m_soundHandles["Wave"] = LoadSoundMem(L"Data/Sound/SE/Wave.mp3");
	m_soundHandles["Shot"] = LoadSoundMem(L"Data/Sound/SE/Shot.mp3");
	m_soundHandles["Blast"] = LoadSoundMem(L"Data/Sound/SE/Blast.mp3");
	m_soundHandles["AddScore"] = LoadSoundMem(L"Data/Sound/SE/AddScore.mp3");
	m_soundHandles["BossBlast"] = LoadSoundMem(L"Data/Sound/SE/BossBlast.mp3");
	//ヒット
	m_soundHandles["SlashHit1"] = LoadSoundMem(L"Data/Sound/SE/Hit/SlashHit1.mp3");
	m_soundHandles["SlashHit2"] = LoadSoundMem(L"Data/Sound/SE/Hit/SlashHit2.mp3");
	m_soundHandles["UltHit"] = LoadSoundMem(L"Data/Sound/SE/Hit/UltHit.mp3");
	m_soundHandles["Hit1"] = LoadSoundMem(L"Data/Sound/SE/Hit/Hit1.mp3");
	m_soundHandles["Hit2"] = LoadSoundMem(L"Data/Sound/SE/Hit/Hit2.mp3");
	m_soundHandles["LastHit"] = LoadSoundMem(L"Data/Sound/SE/Hit/LastHit.mp3");


	//ロードに成功したかチェック
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}
void SoundManager::Update()
{
	for (auto& sound : m_sounds)
	{
		sound->Update();
	}
	//再生が終了したハンドルを消す
	CheckDeleteSound();
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

std::weak_ptr<SE> SoundManager::PlayOnceSE(std::string name)
{
	std::shared_ptr<SE> se;
	auto it = m_soundHandles.find(name);
	if (it != m_soundHandles.end()) {
		// キーがある場合
		se = std::make_shared<SE>(DuplicateSoundMem(m_soundHandles[name]), m_seVolume, false);
		se->Init();
		se->Play();
	}
	return se;
}

std::weak_ptr<SE> SoundManager::PlayOnceSE(int handle)
{
	auto se = std::make_shared<SE>(DuplicateSoundMem(handle), m_seVolume, false);
	se->Init();
	se->Play();
	return se;
}

std::weak_ptr<SE> SoundManager::PlayLoopSE(std::string name)
{
	std::shared_ptr<SE> se;
	auto it = m_soundHandles.find(name);
	if (it != m_soundHandles.end()) {
		// キーがある場合
		se = std::make_shared<SE>(DuplicateSoundMem(m_soundHandles[name]), m_seVolume, true);
		se->Init();
		se->Play();
	}
	return se;
}

std::weak_ptr<SE> SoundManager::PlayLoopSE(int handle)
{
	auto se = std::make_shared<SE>(DuplicateSoundMem(handle), m_seVolume, true);
	se->Init();
	se->Play();
	return se;
}

std::weak_ptr<Voice> SoundManager::PlayVC(std::string name)
{
	std::shared_ptr<Voice> vc;
	auto it = m_soundHandles.find(name);
	if (it != m_soundHandles.end()) {
		// キーがある場合
		vc = std::make_shared<Voice>(DuplicateSoundMem(m_soundHandles[name]), m_seVolume);
		vc->Init();
		vc->Play();
	}
	return vc;
}

std::weak_ptr<Voice> SoundManager::PlayVC(int handle)
{
	auto vc = std::make_shared<Voice>(DuplicateSoundMem(handle), m_seVolume);
	vc->Init();
	vc->Play();
	return vc;
}

void SoundManager::AllPlay()
{
	for (auto& sound : m_sounds)
	{
		sound->Play();
	}
	if (m_bgm)
	{
		m_bgm->Play();
	}
}

void SoundManager::AllStop()
{
	for (auto& sound : m_sounds)
	{
		sound->Stop();
	}
	if (m_bgm)
	{
		m_bgm->Stop();
	}
}

//消滅フラグをチェックして削除
void SoundManager::CheckDeleteSound()
{
	std::list<std::shared_ptr<SoundBase>> deleteSound;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		for (auto& sound : m_sounds)
		{
			if (sound->IsDelete())
			{
				isOneMore = true;
				//削除候補
				deleteSound.emplace_back(sound);
			}
		}
		//削除
		for (auto& sound : deleteSound)
		{
			Exit(sound);
		}
		deleteSound.clear();
		if (!isOneMore)break;
	}
}
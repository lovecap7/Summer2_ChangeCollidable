#include "OptionScene.h"
#include "SceneController.h"
#include "../Main/Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../General/Input.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/Option/ScreenModeUI.h"
#include "../Game/UI/Option/VolumeUI.h"

namespace
{
	//音量調整のリピートフレーム
	constexpr int kRepeateFrame = 2;
}

OptionScene::OptionScene(SceneController& controller):
	SceneBase(controller),
	m_optionIndex(OptionIndex::ScreenMode)
{
}

OptionScene::~OptionScene()
{
}

void OptionScene::Init()
{
	//スクリーンモードUI
	auto screenModeUI = std::make_shared<ScreenModeUI>();
	screenModeUI->Init();
	m_screenModeUI = screenModeUI;
	//マスター
	auto masterUI = std::make_shared<VolumeUI>(300);
	masterUI->Init();
	m_masterUI = masterUI;
	//BGM
	auto bgmUI = std::make_shared<VolumeUI>(400);
	bgmUI->Init();
	m_bgmUI = bgmUI;
	//SE
	auto seUI = std::make_shared<VolumeUI>(500);
	seUI->Init();
	m_seUI = seUI;
	//Voice
	auto voiceUI = std::make_shared<VolumeUI>(600);
	voiceUI->Init();
	m_voiceUI = voiceUI;
	//音量セット
	auto& soundManager = SoundManager::GetInstance();
	m_masterUI.lock()->SetVolume(soundManager.GetMasterVolume());
	m_bgmUI.lock()->SetVolume(soundManager.GetBGMVolume());
	m_seUI.lock()->SetVolume(soundManager.GetSEVolume());
	m_voiceUI.lock()->SetVolume(soundManager.GetVoiceVolume());
}

void OptionScene::Update()
{
	auto& input = Input::GetInstance();
	//項目選び
	int optionIndex = static_cast<int>(m_optionIndex);
	if (input.IsTrigger("Up"))--optionIndex;
	if (input.IsTrigger("Down"))++optionIndex;
	optionIndex = MathSub::ClampInt(optionIndex, static_cast<int>(OptionIndex::ScreenMode), static_cast<int>(OptionIndex::VoiceVolume));
	m_optionIndex = static_cast<OptionIndex>(optionIndex);
	if (input.IsTrigger("B"))
	{
		m_controller.PopScene();
		return;
	}
	//選ばれてる項目に対応した処理
	switch (m_optionIndex)
	{
	case OptionScene::OptionIndex::ScreenMode:
		OptionSreenMode(input);
		break;
	case OptionScene::OptionIndex::MasterVolume:
		OptionMasterVolume(input);
		break;
	case OptionScene::OptionIndex::BGMVolume:
		OptionBGMVolume(input);
		break;
	case OptionScene::OptionIndex::SEVolume:
		OptionSEVolume(input);
		break;
	case OptionScene::OptionIndex::VoiceVolume:
		OptionVoiceVolume(input);
		break;
	default:
		break;
	}
}

void OptionScene::Draw()
{
	
}

void OptionScene::End()
{
	if(!m_screenModeUI.expired()) m_screenModeUI.lock()->Delete();
	if(!m_masterUI.expired()) m_masterUI.lock()->Delete();
	if(!m_bgmUI.expired()) m_bgmUI.lock()->Delete();
	if(!m_seUI.expired()) m_seUI.lock()->Delete();
	if(!m_voiceUI.expired()) m_voiceUI.lock()->Delete();
}

void OptionScene::Restart()
{
}

void OptionScene::OptionSreenMode(Input& input)
{
	if (input.IsTrigger("Right") || input.IsTrigger("Left"))
	{
		//切り替え
		auto& application = Application::GetInstance();
		application.SetWindowMode(!application.IsWindowMode());
	}
}

void OptionScene::OptionMasterVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetMasterVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UIに反映
	m_masterUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetMasterVolume(volume);
}
void OptionScene::OptionBGMVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetBGMVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UIに反映
	m_bgmUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetBGMVolume(volume);
}
void OptionScene::OptionSEVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetSEVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UIに反映
	m_seUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetSEVolume(volume);
}
void OptionScene::OptionVoiceVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetVoiceVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UIに反映
	m_voiceUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetVoiceVolume(volume);
}
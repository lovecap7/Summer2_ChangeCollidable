#include "TitleUI.h"
#include "../../../General/game.h"
#include "../../../General/Input.h"
#include <DxLib.h>

namespace
{
	//タイトル
	constexpr float kTitlePosX = Game::kScreenWidth / 2;
	constexpr float kTitlePosY = 250;
	//PAB
	constexpr float kPressAnyBottunPosX = Game::kScreenWidth / 2;
	constexpr float kPressAnyBottunPosY = 600;
	//点滅全体フレーム
	constexpr int kBlinkingFrame = 60;
	//非表示
	constexpr int kShutFrame = kBlinkingFrame / 2;
}

TitleUI::TitleUI() :
	UIBase(),
	m_countFrame(0),
	m_titleHandle(LoadGraph(L"Data/UI/Title/Title.png")),
	m_titleBackHandle(LoadGraph(L"Data/UI/Title/TitleBack.png")),
	m_pressAnyBottunHandle(LoadGraph(L"Data/UI/Title/PressAnyBottun.png")),
	m_pressAnyBottunBackHandle(LoadGraph(L"Data/UI/Title/PressAnyBottunBack.png")),
	m_isDecide(false)
{

}

TitleUI::~TitleUI()
{
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_titleBackHandle);
	DeleteGraph(m_pressAnyBottunHandle);
	DeleteGraph(m_pressAnyBottunBackHandle);
}

void TitleUI::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerAny())
	{
		m_isDecide = true;
	}
	++m_countFrame;
	if (m_countFrame > kBlinkingFrame)
	{
		m_countFrame = 0;
	}
}

void TitleUI::Draw() const
{
	if (m_isDecide)
	{
		DrawRotaGraph(kTitlePosX, kTitlePosY, 1.0, 0.0, m_titleBackHandle, true);
	}
	DrawRotaGraph(kTitlePosX, kTitlePosY, 1.0, 0.0, m_titleHandle, true);
	if (m_countFrame <= kShutFrame)
	{
		DrawRotaGraph(kPressAnyBottunPosX, kPressAnyBottunPosY, 1.0, 0.0, m_pressAnyBottunBackHandle, true);
		DrawRotaGraph(kPressAnyBottunPosX, kPressAnyBottunPosY, 1.0, 0.0, m_pressAnyBottunHandle, true);
	}
}

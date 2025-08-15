#include "ScreenModeUI.h"
#include "../../../Main/Application.h"
#include "../UIManager.h"
#include "../../../General/game.h"
#include <DxLib.h>

namespace
{
	constexpr int kDrawPosY = 200;
}

ScreenModeUI::ScreenModeUI()
{
	m_isWindowMode = Application::GetInstance().IsWindowMode();
	auto& uiManager = UIManager::GetInstance();
	//ウィンドウモード
	m_windowModeHandle = uiManager.GetImageHandle("WindowMode");
	//フルスクリーン
	m_fullScreenModeHandle = uiManager.GetImageHandle("FullScreenMode");
}

ScreenModeUI::~ScreenModeUI()
{
}

void ScreenModeUI::Update()
{
	m_isWindowMode = Application::GetInstance().IsWindowMode();
}

void ScreenModeUI::Draw() const
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x555555, true);
	if (m_isWindowMode)
	{
		DrawRotaGraph(Game::kScreenCenterX, kDrawPosY, 1.0, 0.0, m_windowModeHandle, true);
	}
	else
	{
		DrawRotaGraph(Game::kScreenCenterX, kDrawPosY, 1.0, 0.0, m_fullScreenModeHandle, true);
	}
}

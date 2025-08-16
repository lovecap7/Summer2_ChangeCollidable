#include "SaveDataUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include <DxLib.h>
#include "../../SaveData/SaveDataManager.h"

SaveDataUI::SaveDataUI():
	UIBase(),
	m_backHandle(UIManager::GetInstance().GetImageHandle("SaveDataBack"))
{

}

SaveDataUI::~SaveDataUI()
{
}

void SaveDataUI::Update()
{
}

void SaveDataUI::Draw() const
{
	if (!m_isDraw)return;
	DrawRotaGraph(Game::kScreenCenterX + 300, Game::kScreenCenterY, 1.0, 0.0, m_backHandle, true);
}

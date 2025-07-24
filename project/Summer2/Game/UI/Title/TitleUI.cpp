#include "TitleUI.h"
#include "../../../General/game.h"
#include <DxLib.h>

TitleUI::TitleUI() :
	UIBase(LoadGraph(L"Data/UI/Title/Title.png"))
{

}

TitleUI::~TitleUI()
{
	DeleteGraph(m_handle);
}

void TitleUI::Update()
{
}

void TitleUI::Draw() const
{
	DrawRotaGraph(Game::kScreenWidth / 2, 250, 1.0, 0.0, m_handle, true);
}

#include "VolumeUI.h"
#include "../../../General/game.h"
#include <DxLib.h>
VolumeUI::VolumeUI(float posY):
	m_pos{ Game::kScreenCenterX ,posY},
	m_volume(0)
{
}

VolumeUI::~VolumeUI()
{
}

void VolumeUI::Update()
{
	m_pos.x = Game::kScreenCenterX + (m_volume - 255 / 2);
}

void VolumeUI::Draw() const
{
	DrawBox(Game::kScreenCenterX - 255 / 2, m_pos.y - 20, Game::kScreenCenterX + 255 / 2, m_pos.y + 20, 0x333333, true);
	DrawCircle(m_pos.x, m_pos.y, 20.0f, 0xff0000, true);
}

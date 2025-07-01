#include "PlayerHPUI.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Player/Player.h"
#include <DxLib.h>

namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 50.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

PlayerHPUI::PlayerHPUI(std::weak_ptr<Player> player) :
	PlayerUIBase(player),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f)
{
}

PlayerHPUI::~PlayerHPUI()
{
}


void PlayerHPUI::Init()
{
	//�v���C���[���������ꍇ����UI���폜
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto hp = m_player.lock()->GetHitPoints().lock();
	//������
	m_viewHp = hp->GetHp();
	m_viewMaxHp = hp->GetMaxHp();
}

void PlayerHPUI::Update()
{
	//�v���C���[���������ꍇ����UI���폜
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto hp = m_player.lock()->GetHitPoints().lock();
	//�X�V
	//�̗͂ɕϓ�����������
	if (m_viewHp != hp->GetHp())
	{
		m_viewHp = hp->GetHp();
	}
	if (m_viewMaxHp != hp->GetMaxHp())
	{
		m_viewMaxHp = hp->GetMaxHp();
	}
}

void PlayerHPUI::Draw() const
{
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0xff5555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x55ff55, true);
}

void PlayerHPUI::End()
{
}


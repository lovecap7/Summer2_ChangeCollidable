#include "PlayerHPUI.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/ActorManager.h"
#include <DxLib.h>

namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 50.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

PlayerHPUI::PlayerHPUI(int handle, std::weak_ptr<Player> player) :
	PlayerUIBase(UIData::PlayerHp, handle, player),
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

void PlayerHPUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�v���C���[�܂��̓{�X���������ꍇ�͂���UI���폜
	if (m_player.expired() || actorManager.lock()->GetBoss().expired())
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
	//�`�悵�Ȃ��Ȃ�return
	if (!m_isDraw)return;
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x555555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x55ff55, true);
}

void PlayerHPUI::End()
{
	DeleteGraph(m_handle);
}


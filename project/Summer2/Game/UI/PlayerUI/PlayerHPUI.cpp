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
	//プレイヤーが消えた場合このUIも削除
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto hp = m_player.lock()->GetHitPoints().lock();
	//初期化
	m_viewHp = hp->GetHp();
	m_viewMaxHp = hp->GetMaxHp();
}

void PlayerHPUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーまたはボスが消えた場合はこのUIも削除
	if (m_player.expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	auto hp = m_player.lock()->GetHitPoints().lock();
	//更新
	//体力に変動があった時
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
	//描画しないならreturn
	if (!m_isDraw)return;
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x555555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x55ff55, true);
}

void PlayerHPUI::End()
{
	DeleteGraph(m_handle);
}


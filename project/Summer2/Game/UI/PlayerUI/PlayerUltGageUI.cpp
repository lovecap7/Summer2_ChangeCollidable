#include "PlayerUltGageUI.h"
#include "../../Actors/Character/Player/UltGage.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/ActorManager.h"
#include <DxLib.h>
namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 110.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

PlayerUltGageUI::PlayerUltGageUI(int handle ,std::weak_ptr<Player> player):
	PlayerUIBase(UIData::PlayerUlt,handle, player),
	m_viewUltGageValue(0.0f),
	m_viewMaxUltGageValue(0.0f)
{
}

PlayerUltGageUI::~PlayerUltGageUI()
{
}

void PlayerUltGageUI::Init()
{
	//プレイヤーが消えた場合このUIも削除
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto ultGage = m_player.lock()->GetUltGage().lock();
	m_viewUltGageValue = ultGage->GetUltGageValue();
	m_viewMaxUltGageValue = ultGage->GetMaxUltGageValue();
}

void PlayerUltGageUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーまたはボスが消えた場合はこのUIも削除
	if (actorManager.lock()->GetPlayer().expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	auto ultGage = m_player.lock()->GetUltGage().lock();
	//更新
	//体力に変動があった時
	if (m_viewUltGageValue != ultGage->GetUltGageValue())
	{
		m_viewUltGageValue = ultGage->GetUltGageValue();
	}
	if (m_viewMaxUltGageValue != ultGage->GetMaxUltGageValue())
	{
		m_viewMaxUltGageValue = ultGage->GetMaxUltGageValue();
	}
}

void PlayerUltGageUI::Draw() const
{
	//描画しないならreturn
	if (!m_isDraw)return;
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxUltGageValue / m_viewMaxUltGageValue) * kBarWidth, kRightPosY, 0x555555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewUltGageValue / m_viewMaxUltGageValue) * kBarWidth, kRightPosY, 0x5555ff, true);
}

void PlayerUltGageUI::End()
{
	DeleteGraph(m_handle);
}

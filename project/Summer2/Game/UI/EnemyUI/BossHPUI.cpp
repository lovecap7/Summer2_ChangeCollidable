#include "BossHPUI.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../../General/game.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/BossArea.h"
#include <DxLib.h>

namespace
{
	constexpr float kLeftPosX = 200.0f;
	constexpr float kLeftPosY = Game::kScreenHeight - 200.0f;
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = Game::kScreenWidth - (kLeftPosX * 2.0f);
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

BossHPUI::BossHPUI(int handle, std::weak_ptr<EnemyBase> enemy) :
	EnemyUIBase(UIData::BossHp, handle, enemy),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f)
{
}

BossHPUI::~BossHPUI()
{
}

void BossHPUI::Init()
{
	//“G‚ªÁ‚¦‚½ê‡‚±‚ÌUI‚àíœ
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
	}
	auto hp = m_enemy.lock()->GetHitPoints().lock();
	//‰Šú‰»
	m_viewHp = hp->GetHp();
	m_viewMaxHp = hp->GetMaxHp();
	//•`‰æ‚µ‚È‚¢
	m_isDraw = false;
}

void BossHPUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ƒ{ƒX‚ªÁ‚¦‚½ê‡‚±‚ÌUI‚àíœ
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
	}
	//ƒ{ƒX•”‰®‚É“ü‚Á‚½Žž•`‰æ
	if (actorManager.lock()->GetBossArea().lock()->IsEntryBossArea())
	{
		m_isDraw = true;
	}
	auto enemy = m_enemy.lock();
	auto hp = enemy->GetHitPoints().lock();
	//XV
	//‘Ì—Í‚É•Ï“®‚ª‚ ‚Á‚½Žž
	if (m_viewHp != hp->GetHp())
	{
		m_viewHp = hp->GetHp();
	}
	if (m_viewMaxHp != hp->GetMaxHp())
	{
		m_viewMaxHp = hp->GetMaxHp();
	}
}

void BossHPUI::Draw() const
{
	//•`‰æ‚µ‚È‚¢‚È‚çreturn
	if (!m_isDraw)return;
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x555555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0xff5555, true);
}

void BossHPUI::End()
{
	DeleteGraph(m_handle);
}

#include "EnemyHPUI.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include <DxLib.h>

namespace
{
	constexpr float kShiftLeftPosX = 70.0f;
	constexpr float kBarHeight = 20.0f;
	constexpr float kBarWidth = kShiftLeftPosX * 2.0f;
}


EnemyHPUI::EnemyHPUI(int handle, std::weak_ptr<EnemyBase> enemy) :
	EnemyUIBase(UIData::EnemyHp, handle, enemy),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f),
	m_pos{}
{
}

EnemyHPUI::~EnemyHPUI()
{
}

void EnemyHPUI::Init()
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
}

void EnemyHPUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//“G‚ªÁ‚¦‚½ê‡‚±‚ÌUI‚àíœ
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
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

	//“G‚Ì“ª•t‹ß‚Éo‚µ‚½‚¢
	m_pos = enemy->GetHPUIPos();
}

void EnemyHPUI::Draw() const
{
	//•`‰æ‚µ‚È‚¢‚È‚çreturn
	if (!m_isDraw)return;
	auto pos = ConvWorldPosToScreenPos(m_pos.ToDxLibVector());
	pos.x -= kShiftLeftPosX;
	DrawBoxAA(pos.x, pos.y, pos.x + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, pos.y - kBarHeight, 0x555555, true);
	DrawBoxAA(pos.x, pos.y, pos.x + (m_viewHp / m_viewMaxHp) * kBarWidth, pos.y - kBarHeight, 0xff5555, true);
}

void EnemyHPUI::End()
{
	DeleteGraph(m_handle);
}

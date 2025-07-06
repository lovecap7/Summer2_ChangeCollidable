#include "TimerUI.h"
#include "../GameRule/Timer.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>

namespace
{
	constexpr int kMillisecond = 60;
	constexpr int kSeconds = 60;
	constexpr int kMinutes = 60;
}

TimerUI::TimerUI(const std::weak_ptr<Timer> timer):
	m_millisecond(0),
	m_seconds(0),
	m_minutes(0),
	m_timer(timer)
{
}

TimerUI::~TimerUI()
{
}

void TimerUI::Init()
{
}

void TimerUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	if (m_timer.expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	//É~Éäïb
	m_millisecond = m_timer.lock()->GetTime();
	//ïbÇçXêV
	m_seconds += m_millisecond / kSeconds;
	//ï™
	m_minutes = m_seconds / kMinutes;
}

void TimerUI::Draw() const
{
	DrawFormatString(Game::kScreenWidth / 2 + 100, 150, 0xffffff, "Timer : %02d.%02d.%02d", m_minutes, m_seconds, m_millisecond);
}

void TimerUI::End()
{
}

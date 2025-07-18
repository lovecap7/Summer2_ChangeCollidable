#include "TimerUI.h"
#include "../GameRule/Timer.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>


TimerUI::TimerUI(int handle,const std::weak_ptr<Timer> timer):
	UIBase(UIData::Timer,handle),
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
}

void TimerUI::Draw() const
{
	if (m_timer.expired())return;
	auto time = m_timer.lock();
	int minSec = time->GetMillisecond();
	int sec = time->GetSeconds();
	int min = time->GetMinutes();
	DrawFormatString(Game::kScreenWidth / 2 + 100, 150, 0xffffff, "Timer : %02d.%02d.%02d",
		min, sec, minSec);
}

void TimerUI::End()
{
	DeleteGraph(m_handle);
}

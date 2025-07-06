#include "Timer.h"

namespace
{
	constexpr int kMaxTime = 999999;
}

Timer::Timer():
	m_time(0)
{
}

Timer::~Timer()
{
}

void Timer::Update()
{
	if (m_time < kMaxTime)
	{
		++m_time;
	}
}

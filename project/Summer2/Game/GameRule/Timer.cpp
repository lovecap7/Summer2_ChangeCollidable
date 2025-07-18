#include "Timer.h"

namespace
{
	constexpr int kMaxTime = 999999;
	constexpr int kMillisecond = 60;
	constexpr int kSeconds = 60;
	constexpr int kMinutes = 60;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Init()
{
	m_time = 0;
	m_millisecond = 0;
	m_seconds = 0;
	m_minutes = 0;
	m_isUpdate = true;
}

void Timer::Update()
{
	if (!m_isUpdate)return;
	if (m_time < kMaxTime)
	{
		//総フレーム
		++m_time;
		//ミリ秒
		++m_millisecond;
		if (m_millisecond >= kMillisecond)
		{
			m_millisecond = 0;
		}
		//秒を更新
		m_seconds = m_time / kSeconds;
		//分
		m_minutes = m_seconds / kMinutes;
	}
}

#pragma once
class Timer
{
public:
	Timer();
	~Timer();
	void Update();
	int GetTime() { return m_time; };
private:
	int m_time;
};


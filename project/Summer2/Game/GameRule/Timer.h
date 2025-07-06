#pragma once
class Timer
{
public:
	Timer();
	~Timer();
	void Update();
	int GetTime() { return m_time; };
	void StopUpdate() { m_isUpdate = false; };
	void StartUpdate() { m_isUpdate = true; };
private:
	int m_time;
	//�X�V�����邩
	bool m_isUpdate;
};


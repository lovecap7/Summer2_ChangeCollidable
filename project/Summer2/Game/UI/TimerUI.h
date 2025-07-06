#pragma once
#include "UIBase.h"
#include <memory>

class ActorManager;
class Timer;
class TimerUI :
    public UIBase
{
public:
    TimerUI(const std::weak_ptr<Timer> timer);
    ~TimerUI();
    void Init()override;
    void Update(const std::weak_ptr<ActorManager> actorManager)override;
    void Draw() const override;
    void End()override;
private:
    int m_millisecond;
    int m_seconds;
    int m_minutes;
    std::weak_ptr<Timer> m_timer;
};


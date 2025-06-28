#pragma once
#include "../Actor.h"

//ƒAƒCƒeƒ€‚ÌŽí—Þ
enum class ItemType : int
{
    Heart = 0,
    Bomb = 1,
    UltGageUp = 2,
    AttackUp = 3,
    DefenseUp = 4,
};
class ActorManager;
class ItemBase abstract:
    public Actor
{
public:
    ItemBase(Shape shape);
    ~ItemBase();
    void Dead(const std::weak_ptr<ActorManager> actorManager) override {};
protected:
 
};


#pragma once
#include "../Actor.h"

//アイテムの種類
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
   
protected:
    //最初の数フレームは当たり判定をしたくないので
    int m_noHitFrame;
};


#pragma once
#include "../Actor.h"

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


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
    //�ŏ��̐��t���[���͓����蔻����������Ȃ��̂�
    int m_noHitFrame;
};


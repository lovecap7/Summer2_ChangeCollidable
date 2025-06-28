#pragma once
#include "../Actor.h"

//�A�C�e���̎��
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
    //�ŏ��̐��t���[���͓����蔻����������Ȃ��̂�
    int m_noHitFrame;
};


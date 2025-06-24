#pragma once
#include "../Actor.h"

//�A�C�e���̎��
enum class ItemKind : int
{
    Heart = 0,
    Bomb = 1,
    UltGageUp = 2,
    AttackUp = 3,
    DefenseUp = 4,
};

class ItemBase abstract:
    public Actor
{
public:
    ItemBase(ItemKind kind);
    ~ItemBase();
    ItemKind GetItemKind() { return m_kind; }
protected:
    //�A�C�e���̎��
    ItemKind m_kind;
};


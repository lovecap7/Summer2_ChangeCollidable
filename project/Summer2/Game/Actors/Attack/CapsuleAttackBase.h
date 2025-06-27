#pragma once
#include "AttackBase.h"
class ActorStateBase;
class CapsuleAttackBase abstract:
    public AttackBase
{
public:
	CapsuleAttackBase(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius);
protected:
	int& m_modelHandle; //モデルハンドル
	int m_startIndex;	//始点インデックス
	int m_endIndex;		//終点インデックス
};


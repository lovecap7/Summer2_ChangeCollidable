#pragma once
#include "AttackBase.h"
class ActorStateBase;
class CapsuleAttackBase abstract:
    public AttackBase
{
public:
	CapsuleAttackBase(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius);
protected:
	int& m_modelHandle; //���f���n���h��
	int m_startIndex;	//�n�_�C���f�b�N�X
	int m_endIndex;		//�I�_�C���f�b�N�X
};


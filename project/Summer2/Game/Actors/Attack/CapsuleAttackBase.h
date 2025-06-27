#pragma once
#include "AttackBase.h"
class Actor;
class CharacterStateBase;
class CapsuleAttackBase abstract:
    public AttackBase
{
public:
	CapsuleAttackBase(std::weak_ptr<Actor> owner);
	virtual ~CapsuleAttackBase() {};

	//�n�_
	void SetStartPos(const Vector3& pos);
	//�I�_
	void SetEndPos(const Vector3& pos);
	//���a
	void SetRadius(float radius);
protected:
};


#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class Camera;
class ActorManager;
class SmallDragonStateBase abstract :
	public CharacterStateBase
{
public:
	SmallDragonStateBase(std::weak_ptr<Actor> owner);
	virtual ~SmallDragonStateBase();
};
//�U���̃f�[�^����������Ƃ��Ɏg��
namespace
{
	std::string kOwnerName = "SmallDragon";
	std::string kAttackName = "Shot";
}


#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class BomberStateBase abstract :
	public CharacterStateBase
{
public:
	BomberStateBase(std::weak_ptr<Actor> owner);
	virtual ~BomberStateBase();
};
//�U���̃f�[�^����������Ƃ��Ɏg��
namespace
{
	std::string kOwnerName = "Bomber";
	std::string kAttackName = "Throw";
}

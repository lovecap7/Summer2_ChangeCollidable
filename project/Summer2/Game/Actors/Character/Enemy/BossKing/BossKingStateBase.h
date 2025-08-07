#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"

namespace
{
	//近距離
	constexpr float kMeleeAttackDistance = 700.0f;
}

class Actor;
class ActorManager;
class BossKingStateBase abstract :
	public CharacterStateBase
{
public:
	BossKingStateBase(std::weak_ptr<Actor> owner);
	virtual ~BossKingStateBase();
protected:
	
};
//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName = "BossKing";
	std::string kRightPunchName = "RightPunch";
	std::string kBeamName = "Beam";
	std::string kJumpAttackName = "JumpAttack";
	std::string kSwingAttackName = "SwingAttack";
}



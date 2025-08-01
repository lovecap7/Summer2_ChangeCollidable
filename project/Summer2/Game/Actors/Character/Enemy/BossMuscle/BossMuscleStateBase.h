#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class ActorManager;
class BossMuscleStateBase abstract :
	public CharacterStateBase
{
public:
	BossMuscleStateBase(std::weak_ptr<Actor> owner, bool isAngry);
	virtual ~BossMuscleStateBase();
protected:
	bool m_isAngry; //怒り状態かどうか
};
//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName = "BossMuscle";
	std::string kBreathName = "Breath";
	std::string kPunchName = "Punch";
	std::string kSweepName = "Sweep";
}



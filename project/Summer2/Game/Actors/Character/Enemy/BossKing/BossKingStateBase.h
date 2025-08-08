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
	BossKingStateBase(std::weak_ptr<Actor> owner, bool isTransformSecond);
	virtual ~BossKingStateBase();
protected:
	//第二形態
	bool m_isTransformSecond;
	//攻撃手段を考える
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
};

//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName = "BossKing";
}



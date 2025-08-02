#pragma once
#include "BossMuscleStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class Input;
class ActorManager;
class Actor;
class BossMuscleStateJumpAttack :
    public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateJumpAttack>
{
public:
	BossMuscleStateJumpAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager);
	~BossMuscleStateJumpAttack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//çUåÇÉfÅ[É^
	AttackData m_attackData;
};


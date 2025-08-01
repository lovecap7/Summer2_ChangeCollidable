#pragma once
#include "BossMuscleStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossMuscleStateAngryIdle :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateAngryIdle>
{
public:
	BossMuscleStateAngryIdle(std::weak_ptr<Actor> owner);
	~BossMuscleStateAngryIdle();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};
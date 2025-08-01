#pragma once
#include "BossMuscleStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class Collidable;
class Strike;
class Input;
class ActorManager;
class Actor;
class BossMuscleStateBeam :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateBeam>
{
public:
	BossMuscleStateBeam(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager);
	~BossMuscleStateBeam();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
	int m_attackCountFrame;
    //UŒ‚‚Ìì¬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
};
#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
class Camera;
class Actor;
class ActorManager;
class Collidable;
class AreaOfEffectAttack;
class BossDragonStatePunchAttack :
	public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStatePunchAttack>
{
public:
	BossDragonStatePunchAttack(std::weak_ptr<Actor> owner);
	~BossDragonStatePunchAttack();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
	int m_attackCountFrame;
	//UŒ‚‚ÌQÆ
	std::weak_ptr<AreaOfEffectAttack> m_attack;
	//UŒ‚‚Ìì¬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//UŒ‚‚ÌˆÊ’u‚ğXV‚·‚é
	void UpdateAttackPos();
};


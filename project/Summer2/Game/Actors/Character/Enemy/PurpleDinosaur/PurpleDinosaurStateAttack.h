#pragma once
#include "PurpleDinosaurStateBase.h"
#include "../../../../../General/Battle.h"

class Camera;
class Actor;
class ActorManager;
class Collidable;
class Strike;
class PurpleDinosaurStateAttack :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateAttack>
{
public:
	PurpleDinosaurStateAttack(std::weak_ptr<Actor> owner);
	~PurpleDinosaurStateAttack();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
	//UŒ‚‚ÌQÆ
	std::weak_ptr<Strike> m_attack;
	//UŒ‚‚Ìì¬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//UŒ‚‚ÌˆÊ’u‚ğXV‚·‚é
	void UpdateAttackPos();
	//UŒ‚‚É‘Oi‚·‚é
	void AttackMove();
};


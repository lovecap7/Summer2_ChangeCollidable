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
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���̎Q��
	std::weak_ptr<AreaOfEffectAttack> m_attack;
	//�U���̍쐬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//�U���̈ʒu���X�V����
	void UpdateAttackPos();
};


#pragma once
#include "SmallDragonStateBase.h"
#include "../../../../../General/Battle.h"

class Camera;
class Actor;
class ActorManager;
class Collidable;
class Bullet;
class SmallDragonStateAttack :
	public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateAttack>
{
public:
	SmallDragonStateAttack(std::weak_ptr<Actor> owner);
	~SmallDragonStateAttack();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���̎Q��
	std::weak_ptr<Bullet> m_attack;
	//�U���̍쐬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
};


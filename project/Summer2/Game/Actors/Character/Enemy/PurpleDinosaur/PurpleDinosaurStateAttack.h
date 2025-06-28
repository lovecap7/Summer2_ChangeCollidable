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
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
	//�U���̎Q��
	std::weak_ptr<Strike> m_attack;
	//�U���̍쐬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//�U���̈ʒu���X�V����
	void UpdateAttackPos();
	//�U�����ɑO�i����
	void AttackMove();
};


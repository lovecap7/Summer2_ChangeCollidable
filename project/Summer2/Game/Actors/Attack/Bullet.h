#pragma once
#include "SphereAttackBase.h"
class Actor;
class CharacterStateBase;
class Bullet:
	public SphereAttackBase
{
public:
	Bullet(std::weak_ptr<Actor> owner);
	virtual ~Bullet() {};
    //�X�V����
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw() const override;
	//�ړ���
	void SetVec(Vector3 vec);
};


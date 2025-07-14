#pragma once
#include "SphereAttackBase.h"
class Actor;
class CharacterStateBase;
class ActorManager;
class Camera;
class Breath :
    public SphereAttackBase
{
public:
	Breath(std::weak_ptr<Actor> owner);
	virtual ~Breath() {};
	//�X�V����
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փˏ���
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw() const override;
	//�ړ���
	void SetVec(Vector3 vec);
};


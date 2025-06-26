#pragma once
#include "AttackBase.h"
class ActorStateBase;
class Slash :
    public AttackBase
{
public:
	Slash(int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw,std::weak_ptr<ActorStateBase> ownerState);
	~Slash();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw()const override;
	//�X�V�����̊m��
	void Complete() override;
	//�m�b�N�o�b�N
	Vector3 GetKnockBackVec(Vector3 other) override;
private:
	
};


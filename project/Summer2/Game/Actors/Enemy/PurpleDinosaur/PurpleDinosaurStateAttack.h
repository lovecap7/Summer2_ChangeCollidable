#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class PurpleDinosaurStateAttack :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateAttack>
{
public:
	PurpleDinosaurStateAttack(std::shared_ptr<PurpleDinosaur> owner);
	~PurpleDinosaurStateAttack();
	void Init()override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //���r
    std::shared_ptr<Collidable> m_leftArm;
    //�U��
    std::shared_ptr<MeleeAttack> m_attack;
    //�U���̓����蔻��쐬
    void CreateAttack();
    //�U���̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
    //�폜
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
    //�U�����̑O�i����
    void AttackMove();
};


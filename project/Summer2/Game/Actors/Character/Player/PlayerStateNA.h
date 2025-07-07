#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
#include <memory>
class Slash;
class ActorManager;
class MyEffect;
class PlayerStateNA :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateNA>
{
public:
    PlayerStateNA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    virtual ~PlayerStateNA();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
protected:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //�U���̎Q��
    std::weak_ptr<Slash> m_attack;
    //�U���̍쐬
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //���ɂ��U���̈ʒu���X�V����
    void UpdateAttackPos();
    //�O�i
    void AttackMove(float speed);
    //�U���f�[�^�̏�����
    void InitAttackData(const std::weak_ptr<ActorManager> actorManager);
    //�U���f�[�^
    AttackData m_attackData;
    //�U����
    int m_comboNum;
    //�a���̎Q��
    std::weak_ptr<MyEffect> m_eff;
};



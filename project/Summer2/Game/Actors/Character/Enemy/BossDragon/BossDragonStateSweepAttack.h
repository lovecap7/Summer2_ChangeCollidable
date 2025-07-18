#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
class Camera;
class Actor;
class ActorManager;
class Collidable;
class Strike;
class BossDragonStateSweepAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateSweepAttack>
{
public:
    BossDragonStateSweepAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
    ~BossDragonStateSweepAttack();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //��]��
    float m_attackRotaAngle;
    //�U���̎Q��
    std::weak_ptr<Strike> m_attack;
    //�U���̍쐬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //�U���̈ʒu���X�V����
    void UpdateAttackPos();
    //�U���f�[�^
    AttackData m_attackData;
};



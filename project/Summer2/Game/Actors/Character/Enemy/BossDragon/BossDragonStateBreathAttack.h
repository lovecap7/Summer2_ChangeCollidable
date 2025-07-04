#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class Camera;
class Actor;
class ActorManager;
class Collidable;
class Breath;
class BossDragonStateBreathAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateBreathAttack>
{
public:
    BossDragonStateBreathAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
    ~BossDragonStateBreathAttack();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //�U���̎Q��
    std::weak_ptr<Breath> m_attack1;
    std::weak_ptr<Breath> m_attack2;
    std::weak_ptr<Breath> m_attack3;
    //�U���̍쐬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //�e�̏�Ԑݒ�
    void SetupBreath(std::weak_ptr<Breath> bullet, float angle);
    //�U���f�[�^
    AttackData m_attackData;
};


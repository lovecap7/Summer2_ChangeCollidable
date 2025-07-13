#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class AreaOfEffectAttack;
class Camera;
class ActorManager;
class TrackActorEffect;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //�A�j���[�V�����̃t���[��
    int m_animCountFrame;
    //�A�j���[�V�����̑��x
    float m_animSpeed;
    //�U���̎Q��
    std::weak_ptr<AreaOfEffectAttack> m_attack;
    //�U���̍쐬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //���ɂ��U���̈ʒu���X�V����
    void UpdateAttackPos();
    //�U���f�[�^
    AttackData m_attackData;
};



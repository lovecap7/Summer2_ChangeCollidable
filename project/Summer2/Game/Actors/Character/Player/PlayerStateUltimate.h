#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class Slash;
class Camera;
class ActorManager;
class MyEffect;
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
    std::weak_ptr<Slash> m_attack;
    //�U���̍쐬
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //�U���f�[�^
    AttackData m_attackData;
};



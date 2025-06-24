#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class Collidable;
class MeleeAttack;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //�A�j���[�V�����̃t���[��
    int m_animCountFrame;
    //�A�j���[�V�����̑��x
    float m_animSpeed;
    //�U��
    std::shared_ptr<MeleeAttack> m_attackUlt;//�K�E�Z
    //��(�U��)�̓����蔻��쐬
    void CreateAttack();
    //��(�U��)�̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
    //�폜
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};



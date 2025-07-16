#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class Slash;
class TrackActorEffect;
class PlayerStateCA :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA>
{
public:
    PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager,AttackData data);
    virtual ~PlayerStateCA();
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
    //�U���f�[�^
    AttackData m_attackData;
    //�a���̎Q��
    std::weak_ptr<TrackActorEffect> m_eff;
    //��]��
    float m_attackRotaAngle;
    //1��]�ɂ�����t���[��
    int m_oneRotaFrame;
};


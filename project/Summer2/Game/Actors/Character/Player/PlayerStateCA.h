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
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚‚ÌQÆ
    std::weak_ptr<Slash> m_attack;
    //UŒ‚‚Ìì¬
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //‘«‚É‚æ‚éUŒ‚‚ÌˆÊ’u‚ğXV‚·‚é
    void UpdateAttackPos();
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
    //aŒ‚‚ÌQÆ
    std::weak_ptr<TrackActorEffect> m_eff;
    //‰ñ“]—Ê
    float m_attackRotaAngle;
    //1‰ñ“]‚É‚©‚©‚éƒtƒŒ[ƒ€
    int m_oneRotaFrame;
};


#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class Camera;
class MyEffect;
class TrackActorEffect;
class PlayerStateCharge :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCharge>
{
public:
    PlayerStateCharge(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    ~PlayerStateCharge();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //タメ時間
    int m_chargeFrame;
    //エフェクト
    std::weak_ptr<MyEffect> m_chargeEff;
    std::weak_ptr<TrackActorEffect> m_levelEff;
    //チャージレベル毎のフレーム
    float m_chargeLevel2Frame;
    float m_chargeLevel3Frame;
    //攻撃データ
    AttackData m_attackData;
};




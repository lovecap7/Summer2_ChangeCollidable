#pragma once
#include "PurpleDinosaurStateBase.h"
#include "../../../Actor.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class Collidable;
class PurpleDinosaurStateChase :
    public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateChase>
{
public:
    PurpleDinosaurStateChase(std::weak_ptr<Actor> owner);
    ~PurpleDinosaurStateChase();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //次の移動ベクトルをレイキャストの結果をもとに取得
    Vector3 GetNextVecFromRayCast(std::shared_ptr<Actor> coll, Actor::TargetData& targetData);
};


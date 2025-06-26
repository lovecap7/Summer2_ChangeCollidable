#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateHit :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateHit>
{
public:
    PlayerStateHit(std::weak_ptr<Player> player);
    ~PlayerStateHit();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//ランダムでヒットアニメーションを選ぶ
	void RandHitAnim();
};


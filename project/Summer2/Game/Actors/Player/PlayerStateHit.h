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
    PlayerStateHit(std::shared_ptr<Player> player);
    ~PlayerStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    void SpeedDown();
	//ランダムでヒットアニメーションを選ぶ
	void RandHitAnim();
};


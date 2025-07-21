#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateNormal :
    public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateNormal>
{
public:
	GameCameraStateNormal(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateNormal() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


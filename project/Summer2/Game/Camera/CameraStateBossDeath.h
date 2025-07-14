#pragma once
#include "CameraStateBase.h"
class CameraStateBossDeath :
	public CameraStateBase, public std::enable_shared_from_this<CameraStateBossDeath>
{
public:
	CameraStateBossDeath(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~CameraStateBossDeath() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


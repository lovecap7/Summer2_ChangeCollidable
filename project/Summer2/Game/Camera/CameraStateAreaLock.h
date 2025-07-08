#pragma once
#include "CameraStateBase.h"
class CameraStateAreaLock :
    public CameraStateBase, public std::enable_shared_from_this<CameraStateAreaLock>
{
public:
	CameraStateAreaLock(std::weak_ptr<Camera> camera);
	virtual ~CameraStateAreaLock() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


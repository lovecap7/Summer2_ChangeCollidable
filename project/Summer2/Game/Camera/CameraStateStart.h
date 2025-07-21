#pragma once
#include "CameraStateBase.h"
class Camera;
class ActorManager;
class CameraStateStart :
	public CameraStateBase, public std::enable_shared_from_this<CameraStateStart>
{
public:
	CameraStateStart(std::weak_ptr<Camera> camera);
	virtual ~CameraStateStart() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
};
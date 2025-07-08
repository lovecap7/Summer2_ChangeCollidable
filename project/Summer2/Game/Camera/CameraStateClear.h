#pragma once
#include "CameraStateBase.h"
class ActorManager;
class CameraStateClear :
	public CameraStateBase, public std::enable_shared_from_this<CameraStateClear>
{
public:
	CameraStateClear(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~CameraStateClear() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
	int m_rotaFrame;
};


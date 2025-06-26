#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Input;
class Camera;
class ActorManager;
class ActorStateBase abstract
{
public:
	ActorStateBase();
	virtual ~ActorStateBase();
	//‰Šú‰»ˆ—
	virtual void Init()abstract;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//Ÿ‚Ìó‘Ô‚ğæ“¾
	std::shared_ptr<ActorStateBase> GetNextState() { return m_nextState; };
	//ó‘Ô•Ï‰»
	virtual void ChangeState(std::shared_ptr<ActorStateBase> nextState);
private:
	//Ÿ‚Ìó‘Ô
	std::shared_ptr<ActorStateBase> m_nextState;
};


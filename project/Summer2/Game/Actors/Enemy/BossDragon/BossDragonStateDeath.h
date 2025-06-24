#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class ActorManager;
class BossDragon;
class BossDragonStateDeath :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateDeath>
{
public:
	BossDragonStateDeath(std::shared_ptr<BossDragon> owner);
	~BossDragonStateDeath();
	void Init() override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};


#pragma once
#include "BossDragonStateBase.h"
class Camera;
class ActorManager;
class Input;
class BossDragon;
class BossDragonStateIdle :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateIdle>
	
{
public:
	BossDragonStateIdle(std::shared_ptr<BossDragon> owner);
	~BossDragonStateIdle();
	void Init()override;
	void Update(const Input & input, const std::unique_ptr<Camera>&camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
	//‚Ç‚ÌUŒ‚‚ğ‚·‚é‚©l‚¦‚é
	void ThinkAttack();
};
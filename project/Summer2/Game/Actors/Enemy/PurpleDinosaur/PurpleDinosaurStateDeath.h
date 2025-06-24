#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class ActorManager;
class PurpleDinosaur;
class PurpleDinosaurStateDeath :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateDeath>
{
public:
	PurpleDinosaurStateDeath(std::shared_ptr<PurpleDinosaur> owner);
	~PurpleDinosaurStateDeath();
	void Init()override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};


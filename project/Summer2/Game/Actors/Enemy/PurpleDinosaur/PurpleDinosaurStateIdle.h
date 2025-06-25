#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class ActorManager;
class PurpleDinosaurStateIdle :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateIdle>
{
public:
	PurpleDinosaurStateIdle(std::weak_ptr<PurpleDinosaur> owner);
	~PurpleDinosaurStateIdle();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};
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
	PurpleDinosaurStateDeath(std::weak_ptr<PurpleDinosaur> owner);
	~PurpleDinosaurStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera) override;
private:
	//�������Ă���
	void SpeedDown();
};


#pragma once
#include "SphereAttackBase.h"
class Actor;
class CharacterStateBase;
class ActorManager;
class Camera;
class Breath :
    public SphereAttackBase
{
public:
	Breath(std::weak_ptr<Actor> owner);
	virtual ~Breath() {};
	//XVˆ—
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//•`‰æ
	void Draw() const override;
	//ˆÚ“®—Ê
	void SetVec(Vector3 vec);
};


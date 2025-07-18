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
	//更新処理
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突処理
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw() const override;
	//移動量
	void SetVec(Vector3 vec);
};


#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class Model;
class Sky :
    public Actor,public std::enable_shared_from_this<Sky>
{
public:
	Sky(int handle);
	~Sky();
    //“o˜^ˆ—
    void Entry(std::shared_ptr<ActorManager> actorManager) override;
    //“o˜^‰ğœ
    void Exit(std::shared_ptr<ActorManager> actorManager) override;
    void Init()override {};
    void Update(const Input& input, const std::unique_ptr<Camera>& camera,const std::shared_ptr<ActorManager> actorManager) override;
    void Gravity(const Vector3& gravity)override {};
    void OnHitColl(const std::shared_ptr<Collidable>& other)override {};
    void Draw()const override;
    void Complete() override {};
private:
	//ƒ‚ƒfƒ‹
	std::shared_ptr<Model> m_model;
};


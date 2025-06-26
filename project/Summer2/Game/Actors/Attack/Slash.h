#pragma once
#include "AttackBase.h"
class ActorStateBase;
class Slash :
    public AttackBase
{
public:
	Slash(int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw,std::weak_ptr<ActorStateBase> ownerState);
	~Slash();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw()const override;
	//更新処理の確定
	void Complete() override;
	//ノックバック
	Vector3 GetKnockBackVec(Vector3 other) override;
private:
	
};


#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Camera;
class AttackUp :
	public ItemBase
{
public:
	AttackUp(int modelHandle, Vector3 pos);
	virtual ~AttackUp();
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
	//死亡処理
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//終了処理
	void End()override;
private:

};


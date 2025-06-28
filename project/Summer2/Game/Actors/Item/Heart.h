#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Camera;
class Heart :
	public ItemBase
{
public:
	Heart(int modelHandle, Vector3 pos);
	virtual ~Heart();
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
private:
	//最初の数フレームは当たり判定をしたくないので
	int m_noHitFrame;
};


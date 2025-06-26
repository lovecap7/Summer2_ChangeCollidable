#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class UltGage;
class Camera;
class PlayerStateBase;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//入力中の方向キー
	Vector2 GetStickVec() { return m_stickVec; };
	//必殺技ゲージ
	std::shared_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//リジッドボディ
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//コリジョン
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//コリジョンの状態を設定
	void SetCollState(CollisionState collState) { m_collState = collState; }
private:
	//プレイヤーの状態
	std::shared_ptr<PlayerStateBase> m_state;
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
	//必殺技ゲージ
	std::shared_ptr<UltGage> m_ultGage;
};


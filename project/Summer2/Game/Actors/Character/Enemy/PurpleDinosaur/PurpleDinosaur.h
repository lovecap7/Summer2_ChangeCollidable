#pragma once
#include "../EnemyBase.h"
class ActorManager;
class Input;
class Camera;
class EnemyManager;
class AttackManager;
class ActorManager;
class CharacterStateBase;
class PurpleDinosaur :
    public EnemyBase
{
public:
	PurpleDinosaur(int modelHandle, Vector3 pos);
	~PurpleDinosaur();
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
	//リジッドボディ
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//コリジョン
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//コリジョンの状態を設定
	void SetCollState(CollisionState collState) { m_collState = collState; }
	//ターゲットのデータ
	TargetData GetTargetData() const { return m_targetData; };
private:
	
};


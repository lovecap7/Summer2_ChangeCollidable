#pragma once
#include "../EnemyBase.h"
class ActorManager;
class Input;
class Camera;
class EnemyManager;
class UIManager;
class BossDragonStateBase;
class BossDragon :
	public EnemyBase, public std::enable_shared_from_this<BossDragon>
{
public:
	BossDragon(int modelHandle, Vector3 pos);
	~BossDragon();
	//登録処理
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//登録解除
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager) override;
	//重力
	void Gravity(const Vector3& gravity)override;
	//衝突イベント
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//描画
	void Draw()const override;
	//更新処理の確定
	void Complete() override;
	//攻撃のクールタイムを取得
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//攻撃のクールタイムをセット
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
private:
	//自分の状態
	std::shared_ptr<BossDragonStateBase> m_state;
	//攻撃できるまでのクールタイム
	int m_attackCoolTime;
	//やられ判定の更新
	void UpdateHurtPoint()override;
	//攻撃のクールタイムを更新
	void UpdateAttackCoolTime();
};


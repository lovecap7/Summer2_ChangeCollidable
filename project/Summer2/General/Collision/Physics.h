#pragma once
#include <list>
#include <vector>
#include <memory>
#include "Collidable.h"
/// <summary>
/// 当たり判定のシングルトンクラス
/// </summary>
class Collidable;
class CollisionChecker;
class FixNextPosition;
class Physics final
{
private:
	//シングルトンの準備
	Physics() = default;
	~Physics() = default;
	//コピー禁止
	Physics(const Physics&) = delete;
	Physics& operator = (const Physics&) = delete;
	//ムーブ禁止
	Physics(Physics&&) = delete;
	Physics& operator = (Physics&&) = delete;
public:
	//インスタンスを取得
	static Physics& GetInstance()
	{
		static Physics instance;
		return instance;
	}
	
	//初期化処理
	void Init();
	//登録
	void Entry(std::shared_ptr<Collidable> collidable);
	//解除
	void Exit(std::shared_ptr<Collidable> collidable);
	//衝突処理
	void Update();
	//登録しているCollidableを全て削除
	void Reset();


	//更新を止める
	void StopUpdate() { m_isUpdate = false; }
	//更新を始める
	void StartUpdate() { m_isUpdate = true; }
	//更新を遅らせる
	void DelayUpdate(int frame);
	//指定された範囲内のCollidableを返す(X座標を見て)
	std::list<std::weak_ptr<Collidable>> GetAreaXCollidable(float startX,float endX);
private:
	//当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
	//衝突処理
	std::shared_ptr<FixNextPosition> m_collProcessor;
private:
	//登録されているコライダブルのリスト
	std::list<std::shared_ptr<Collidable>> m_collidables;

	// OnCollideの衝突通知のためのデータ
	struct OnCollideInfo
	{
		std::shared_ptr<Collidable> owner;
		std::shared_ptr<Collidable> colider;
		void OnCollide() { owner->OnCollide(colider); }
	};

	//重力
	void Gravity();

	//更新をするか
	bool m_isUpdate;
	//遅延
	int m_delayFrame;
};


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
class CollisionProcess;
class Physics final
{
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


	//更新を止める
	void StopUpdate() { m_isUpdate = false; }

	//更新を始める
	void StartUpdate() { m_isUpdate = true; }
private:
	//当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
	//衝突処理
	std::shared_ptr<CollisionProcess> m_collProcessor;
private:
	//シングルトンの準備
	Physics() = default;
	~Physics();
	Physics(const Physics&) = delete;
	Physics& operator = (const Physics&) = delete;
	Physics(Physics&&) = delete;
	Physics& operator = (Physics&&) = delete;
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
};


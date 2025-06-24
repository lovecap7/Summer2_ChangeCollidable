#pragma once
#include <memory>
#include <vector>
class Actor;
class Player;
class CollisionChecker;
class EnemyBase;
class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> player);
	virtual ~EnemyManager() {};
	//登録
	void Entry(std::shared_ptr<EnemyBase> enemy);
	//登録解除
	void Exit(std::shared_ptr<EnemyBase> enemy);
	//更新処理
	void Update();
private:
	//トリガーの当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
	//プレイヤーのポインタ
	std::shared_ptr<Player> m_player;
	//敵のポインタ
	std::vector<std::shared_ptr<EnemyBase>> m_enemies;
private:
	//プレイヤーの索敵
	void SearchPlayer();
};


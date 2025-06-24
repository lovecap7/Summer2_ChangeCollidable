#pragma once
#include <memory>
#include <vector>

class Player;
class Actor;
class CollisionChecker;
class PlayerManager
{
public:
	PlayerManager(std::shared_ptr<Player> player);
	~PlayerManager();
	void Update(std::vector<std::shared_ptr<Actor>> actors);
private:
	void SearchTarget(std::vector<std::shared_ptr<Actor>> actors);
	//プレイヤー
	std::shared_ptr<Player> m_player;
	//トリガーの当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_triggerChecker;
};


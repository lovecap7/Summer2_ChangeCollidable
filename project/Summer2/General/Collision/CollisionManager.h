#pragma once
#include <vector>
#include <memory>
class Actor;
class CollisionChecker;
class CollisionProcess;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	void Update(std::vector<std::shared_ptr<Actor>> actors);
private:
	//当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
	//衝突処理
	std::shared_ptr<CollisionProcess> m_collProcessor;
};


#pragma once
#include <vector>
#include <memory>
class AttackBase;
class CollisionChecker;
class Actor;
class AttackManager
{
public:
	AttackManager();
	~AttackManager();
	void Update(std::vector<std::shared_ptr<Actor>> actors);
	void Draw()const;
	//UŒ‚‚ğ“o˜^
	void Entry(std::shared_ptr<AttackBase> attack);
	//“o˜^‰ğœ
	void Exit(std::shared_ptr<AttackBase> attack);
private:
	//UŒ‚
	std::vector<std::shared_ptr<AttackBase>> m_attacks;
	//“–‚½‚è”»’è‚Ìƒ`ƒFƒbƒN‚ğ‚·‚éƒNƒ‰ƒX
	std::shared_ptr<CollisionChecker> m_collChecker;
};


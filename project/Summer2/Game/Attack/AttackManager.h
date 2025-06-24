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
	//�U����o�^
	void Entry(std::shared_ptr<AttackBase> attack);
	//�o�^����
	void Exit(std::shared_ptr<AttackBase> attack);
private:
	//�U��
	std::vector<std::shared_ptr<AttackBase>> m_attacks;
	//�����蔻��̃`�F�b�N������N���X
	std::shared_ptr<CollisionChecker> m_collChecker;
};


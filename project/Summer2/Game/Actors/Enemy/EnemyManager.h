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
	//�o�^
	void Entry(std::shared_ptr<EnemyBase> enemy);
	//�o�^����
	void Exit(std::shared_ptr<EnemyBase> enemy);
	//�X�V����
	void Update();
private:
	//�g���K�[�̓����蔻��̃`�F�b�N������N���X
	std::shared_ptr<CollisionChecker> m_collChecker;
	//�v���C���[�̃|�C���^
	std::shared_ptr<Player> m_player;
	//�G�̃|�C���^
	std::vector<std::shared_ptr<EnemyBase>> m_enemies;
private:
	//�v���C���[�̍��G
	void SearchPlayer();
};


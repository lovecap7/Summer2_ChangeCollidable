#pragma once
#include <memory>
#include <list>
class UIBase;
class ActorManager;
class Player;
class EnemyBase;
class Score;
class UIManager
{
public:
	UIManager();
	~UIManager();
	void Init();
	void Update(const std::weak_ptr<ActorManager> actorManager);
	void Draw()const;
	void End();
	//�v���C���[�֘A��UI�쐬
	void CreatePlayerUI(const std::weak_ptr<Player> player);
	//�{�XUI
	void CreateBossUI(const std::weak_ptr<EnemyBase> boss);
	//�G��UI
	void CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy);
	//�X�R�A
	void CreateScoreUI(const std::weak_ptr<Score> score);
private:
	//UI
	std::list<std::shared_ptr<UIBase>> m_uis;
	//UI�ǉ�
	void AddUI(std::shared_ptr<UIBase> ui);
	//�폜�\���UI���폜
	void CheckDelete();
};


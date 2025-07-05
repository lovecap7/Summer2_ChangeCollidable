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
	//プレイヤー関連のUI作成
	void CreatePlayerUI(const std::weak_ptr<Player> player);
	//ボスUI
	void CreateBossUI(const std::weak_ptr<EnemyBase> boss);
	//敵のUI
	void CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy);
	//スコア
	void CreateScoreUI(const std::weak_ptr<Score> score);
private:
	//UI
	std::list<std::shared_ptr<UIBase>> m_uis;
	//UI追加
	void AddUI(std::shared_ptr<UIBase> ui);
	//削除予定のUIを削除
	void CheckDelete();
};


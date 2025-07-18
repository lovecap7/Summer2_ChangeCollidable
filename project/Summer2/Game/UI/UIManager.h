#pragma once
#include <memory>
#include <list>
#include <map>
#include <string>
class UIBase;
class ActorManager;
class Player;
class EnemyBase;
class Score;
class Timer;
class UIManager
{
public:
	UIManager();
	~UIManager();
	void Init();
	void Update(const std::weak_ptr<ActorManager> actorManager);
	void Draw()const;
	void End();
	//再スタート
	void Restart();
	//プレイヤー関連のUI作成
	void CreatePlayerUI(const std::weak_ptr<Player> player);
	//ボスUI
	void CreateBossUI(const std::weak_ptr<EnemyBase> boss);
	//敵のUI
	void CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy);
	//スコア
	void CreateScoreUI(const std::weak_ptr<Score> score);
	//タイマー
	void CreateTimerUI(const std::weak_ptr<Timer> timer);
	//ハンドルロード
	void LoadHandle();
	//UIのハンドル削除
	void AllDeleteHandle();
private:
	//UI
	std::list<std::shared_ptr<UIBase>> m_uis;
	//UI追加
	void AddUI(std::shared_ptr<UIBase> ui);
	//UIの削除
	void AllDeleteUIs();
	//削除予定のUIを削除
	void CheckDelete();
	//ハンドル
	std::map<std::string, int> m_handles;
};


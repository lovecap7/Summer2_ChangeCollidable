#pragma once
#include <memory>
#include <list>
#include <map>
#include <string>
class UIBase;
class Player;
class EnemyBase;
class Score;
class Timer;
class BossHPUI;
class UIManager final
{
private:
	//シングルトンの準備
	UIManager() = default;
	~UIManager() = default;
	//コピー禁止
	UIManager(const UIManager&) = delete;
	UIManager& operator = (const UIManager&) = delete;
	//ムーブ禁止
	UIManager(UIManager&&) = delete;
	UIManager& operator = (UIManager&&) = delete;
public:
	//インスタンスを取得
	static UIManager& GetInstance()
	{
		static UIManager instance;
		return instance;
	}
	//登録
	void Entry(std::shared_ptr<UIBase> ui);
	//解除
	void Exit(std::shared_ptr<UIBase> ui);
	void Init();
	void Update();
	void Draw()const;
	void End();
	//再スタート
	void Reset();
	//プレイヤー関連のUI
	void CreatePlayerUI(const std::weak_ptr<Player> player);
	//ボスUI
	std::weak_ptr<BossHPUI> CreateBossUI(const std::weak_ptr<EnemyBase> boss);
	//敵UI
	void CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy);
	//スコア
	void CreateScoreUI(const std::weak_ptr<Score> score);
	//タイマー
	void CreateTimerUI(const std::weak_ptr<Timer> timer);
	//画像ハンドル取得
	int GetImageHandle(const std::string& name)const;
	//テキストハンドル取得
	int GetTextHandle(const std::string& name)const;
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
	//画像ハンドル
	std::map<std::string, int> m_imageHandles;
	//テキストハンドル
	std::map<std::string, int> m_textHandles;
};


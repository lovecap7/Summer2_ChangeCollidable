#pragma once
#include <list>
#include <memory>

class Player;
class Camera;
class Actor;
class StageSetup;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager();
	~ActorManager();
	//初期化
	void Init();
	//更新
	void Update(const std::weak_ptr<Camera> camera);
	//描画
	void Draw() const;
	//終了処理
	void End();
	//プレイヤーを取得
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	//追加予定のアクターを追加
	void AddNextActor(std::shared_ptr<Actor> actor);
private:
	//アクターを追加
	void AddActor(std::shared_ptr<Actor> actor);
	//追加予定のアクターを実装
	void CheckNextAddActors();
	//削除予定のアクターを削除
	void CheckDeleteActors();
private:
	//アクター
	std::list<std::shared_ptr<Actor>> m_actors;
	//追加予定のアクター
	std::list<std::shared_ptr<Actor>> m_nextAddActors;
	//プレイヤー
	std::shared_ptr<Player> m_player;
	//ステージセットアップ
	std::shared_ptr<StageSetup> m_stageSetup;
	//ID
	int m_actorId;//割り振る番号
};


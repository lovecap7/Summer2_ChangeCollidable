#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
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
	//プレイヤーに近い敵を取得
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//追加予定のアクターを追加
	void AddNextActor(std::shared_ptr<Actor> actor);
	//攻撃を作成して参照を返す
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//アイテムの追加
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
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
	std::unique_ptr<StageSetup> m_stageSetup;
	//ID
	int m_actorId;//割り振る番号
private:
	//ハンドル
	int m_heartHandle;			//回復
	int m_bombHandle;			//爆弾
	int m_ultGageUpHandle;		//必殺ゲージアップ
	int m_defenseHandle;		//耐久アップ
};


#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
class Player;
class Camera;
class Actor;
class StageSetup;
class BossArea;
class EnemyBase;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager();
	~ActorManager();
	//初期化
	void Init(std::unique_ptr<StageSetup>& stageSetup);
	//更新
	void Update(const std::weak_ptr<Camera> camera);
	//描画
	void Draw() const;
	//終了処理
	void End();
	//プレイヤーを取得
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	//ボスを取得
	std::weak_ptr<EnemyBase> GetBoss() const { return m_boss; }
	//プレイヤーに近い敵を取得
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//ボス部屋に入ったか
	bool IsEntryBossArea()const { return m_bossArea.expired(); };
	//追加予定のアクターを追加
	void AddNextActor(std::shared_ptr<Actor> actor);
	//攻撃を作成して参照を返す
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//アイテムの追加
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
	//雑魚敵をすべて削除
	void AllDeleteNormalEnemy();
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
	std::weak_ptr<Player> m_player;
	//ボス
	std::weak_ptr<EnemyBase> m_boss;
	//ボス部屋
	std::weak_ptr<BossArea> m_bossArea;
	//ID
	int m_actorId;//割り振る番号
private:
	//ハンドル
	int m_heartHandle;			//回復
	int m_bombHandle;			//爆弾
	int m_ultGageUpHandle;		//必殺ゲージアップ
	int m_attackUpHandle;		//攻撃アップ
	int m_defenseUpHandle;		//耐久アップ
};


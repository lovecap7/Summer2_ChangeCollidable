#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
#include "../../General/CSVDataLoader.h"
#include "../../Scene/StageScene.h"
#include <map>

//キャラクターの種類
enum class CharacterType : int
{
	Player			= 0,
	PurpleDinosaur	= 1,
	SmallDragon		= 2,
	Bomber			= 3,
	BossDragon		= 4,
};
//アイテムの種類
enum class ItemType : int
{
	Heart			= 0,
	Bomb			= 1,
	UltGageUp		= 2,
	AttackUp		= 3,
	DefenseUp		= 4,
};
//攻撃の種類
enum class AttackType : int
{
	Slash			= 0,
	Strike			= 1,
	AreaOfEffect	= 2,
	Bullet			= 3,
	Blast			= 4,
	Breath			= 5,
};

class Player;
class Camera;
class Actor;
class BossArea;
class EnemyBase;
class UIManager;
class Score;
class CSVDataLoader;
class CharacterBase;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(Stage::StageIndex index,std::weak_ptr<UIManager> uiManager);
	~ActorManager();
	//登録
	void Entry(std::shared_ptr<Actor> actor);
	//解除
	void Exit(std::shared_ptr<Actor> actor);
	//初期化
	void Init();
	//更新
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<Score> score);
	//描画
	void Draw() const;
	//終了処理
	void End();

	//追加予定のアクターを追加
	void AddNextActor(std::shared_ptr<Actor> actor);
	//キャラクターを作成
	std::weak_ptr<CharacterBase> CreateCharacter(CharacterType ch, Vector3 pos);
	//攻撃を作成して参照を返す
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//アイテムの追加
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
	//雑魚敵をすべて削除
	void AllDeleteNormalEnemy();

	//プレイヤーを取得
	std::weak_ptr<Player> GetPlayer() const { return m_player; };
	//ボスを取得
	std::weak_ptr<EnemyBase> GetBoss() const { return m_boss; };
	//ボス部屋
	std::weak_ptr<BossArea> GetBossArea() const { return m_bossArea; };
	//プレイヤーに近い敵を取得
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//攻撃データを返す
	AttackData GetAttackData(std::string& ownerName, std::string& attackName);
private:
	//追加予定のアクターを実装
	void CheckNextAddActors();
	//削除予定のアクターを削除
	void CheckDeleteActors(const std::weak_ptr<Score> score);
	//攻撃データの作成
	void CreateAttackData();
	//ハンドルロード
	void LoadHandle();
	//ステージの準備
	void LoadStage(Stage::StageIndex index);
private:
	//アクター
	std::list<std::shared_ptr<Actor>> m_actors;
	//追加予定のアクター
	std::list<std::shared_ptr<Actor>> m_nextAddActors;
	//CSVロード
	std::unique_ptr<CSVDataLoader> m_csvLoader;
	//プレイヤー
	std::weak_ptr<Player> m_player;
	//ボス
	std::weak_ptr<EnemyBase> m_boss;
	//ボス部屋
	std::weak_ptr<BossArea> m_bossArea;
	//ID
	int m_actorId;//割り振る番号
	//攻撃データ
	std::vector<AttackData> m_attackDatas;
	//UIの参照
	std::weak_ptr<UIManager> m_uiManager;
private:
	//ハンドル
	std::map<std::string, int> m_handles;
};


#include "ActorManager.h"
#include "Stage/BossArea.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"
#include "../UI/UIManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>
#include <cassert>
//配置データ
#include "../../General/CSVDataLoader.h"
//アクター
#include "Actor.h"
//プレイヤー
#include "Character/Player/Player.h"
//敵
#include "Character/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "Character/Enemy/SmallDragon/SmallDragon.h"
#include "Character/Enemy/BossDragon/BossDragon.h"
#include "Character/Enemy/Bomber/Bomber.h"
#include "Character/Enemy/EnemyBase.h"
//ステージ
#include "Stage/InvisibleWall.h"
#include "Stage/StageObjectCollision.h"
#include "Stage/StageObjectDraw.h"
#include "Stage/Sky.h"
#include "Stage/BossArea.h"
//アイテム
#include "Item/Heart.h"
#include "Item/UltGageUp.h"
#include "Item/Bomb.h"
#include "Item/AttackUp.h"
#include "Item/DefenseUp.h"
//攻撃
#include "Attack/Slash.h"
#include "Attack/Strike.h"
#include "Attack/AreaOfEffectAttack.h"
#include "Attack/Bullet.h"
#include "Attack/Blast.h"
#include "Attack/Breath.h"

ActorManager::ActorManager(Stage::StageIndex index,std::weak_ptr<UIManager> uiManager):
	m_actorId(0),
	m_uiManager(uiManager)
{
	//ハンドルロード
	LoadHandle();
	//ステージ情報をロード
	LoadStage(index);
	//攻撃の情報を作成
	CreateAttackData();
}

ActorManager::~ActorManager()
{
}

//アクターを追加
void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//すでに登録されているならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())return;
	//アクターの初期化
	actor->Init();
	//アクターのIDを設定
	actor->SetID(m_actorId);
	m_actorId++;
	//アクターを追加
	m_actors.emplace_back(actor);
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//登録されていないならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it == m_actors.end())return;
	actor->End();
	m_actors.remove(actor);
}


void ActorManager::Init()
{
	//アクターを実装
	CheckNextAddActors();
}

void ActorManager::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<Score> score)
{
	//新規アクターの追加
	CheckNextAddActors();
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(camera,shared_from_this());
	}
	//ボスが倒されたとき
	if (m_boss.expired())
	{
		//プレイヤーの体力からスコアを加算
		score.lock()->AddHPScore(m_player.lock()->GetHitPoints());
	}
	//消滅フラグチェック
	CheckDeleteActors(score);
}

void ActorManager::Draw() const
{
	//アクターの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
}

void ActorManager::End()
{
	//メモリを解放
	//アクターの終了処理
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (auto& actor : m_actors)
	{
		deleteActer.emplace_back(actor);
	}
	for (auto& actor : deleteActer)
	{
		Exit(actor);
	}
	deleteActer.clear();
	m_nextAddActors.clear();
	m_player.reset();
	m_boss.reset();
	//ハンドル
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_purpleDinosaurHandle);
	MV1DeleteModel(m_smallDragonHandle);
	MV1DeleteModel(m_bossDragonHandle);
	MV1DeleteModel(m_bomberHandle);
	MV1DeleteModel(m_pathHandle);
	MV1DeleteModel(m_cubeHandle);
	MV1DeleteModel(m_cylinderHandle);
	MV1DeleteModel(m_skyHandle);
	MV1DeleteModel(m_planeHandle);
	MV1DeleteModel(m_blockGrassHandle);
	MV1DeleteModel(m_heartHandle);
	MV1DeleteModel(m_bombHandle);
	MV1DeleteModel(m_ultGageUpHandle);
	MV1DeleteModel(m_attackUpHandle);
	MV1DeleteModel(m_defenseUpHandle);
}

//新規アクターの追加予定を受け取る(public)
void ActorManager::AddNextActor(std::shared_ptr<Actor> actor)
{
	//追加予定のアクターを追加
	m_nextAddActors.emplace_back(actor);
}

std::weak_ptr<CharacterBase> ActorManager::CreateCharacter(CharacterType ch, Vector3 pos)
{
	//キャラクターを作成
	std::shared_ptr<CharacterBase> chara;
	//必要なUIを作成するために
	auto uiManager = m_uiManager.lock();
	switch (ch)
	{
	case CharacterType::Player:
		//プレイヤー作成
		chara = std::make_shared<Player>(m_playerHandle, pos);
		//プレイヤーに必要なUI作成
		uiManager->CreatePlayerUI(std::dynamic_pointer_cast<Player>(chara));
		break;
	case CharacterType::PurpleDinosaur:
		chara = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_purpleDinosaurHandle), pos);
		//敵に必要なUI作成
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::SmallDragon:
		chara = std::make_shared<SmallDragon>(MV1DuplicateModel(m_smallDragonHandle), pos);
		//敵に必要なUI作成
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::Bomber:
		chara = std::make_shared<Bomber>(MV1DuplicateModel(m_bomberHandle), pos);
		//敵に必要なUI作成
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::BossDragon:
		chara = std::make_shared<BossDragon>(MV1DuplicateModel(m_bossDragonHandle), pos);
		//ボスに必要なUI作成
		uiManager->CreateBossUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	default:
		break;
	}
	//キャラクターを入れる
	AddNextActor(chara);
	return chara;
}
std::weak_ptr<AttackBase> ActorManager::CreateAttack(AttackType at, std::weak_ptr<Actor> owner)
{
	//攻撃を作成
	std::shared_ptr<AttackBase> attack;
	switch (at)
	{
	case AttackType::Slash:
		attack = std::make_shared<Slash>(owner);
		break;
	case AttackType::Strike:
		attack = std::make_shared<Strike>(owner);
		break;
	case AttackType::AreaOfEffect:
		attack = std::make_shared<AreaOfEffectAttack>(owner);
		break;
	case AttackType::Bullet:
		attack = std::make_shared<Bullet>(owner);
		break;
	case AttackType::Blast:
		attack = std::make_shared<Blast>(owner);
		break;
	case AttackType::Breath:
		attack = std::make_shared<Breath>(owner);
		break;
	default:
		break;
	}
	//攻撃を入れる
	AddNextActor(attack);
	return attack;
}

std::weak_ptr<ItemBase> ActorManager::CreateItem(ItemType it, Vector3 pos)
{
	//攻撃を作成
	std::shared_ptr<ItemBase> item;
	switch (it)
	{
	case ItemType::Heart:
		item = std::make_shared<Heart>(MV1DuplicateModel(m_heartHandle), pos);
		break;
	case ItemType::Bomb:
		item = std::make_shared<Bomb>(MV1DuplicateModel(m_bombHandle), pos);
		break;
	case ItemType::UltGageUp:
		item = std::make_shared<UltGageUp>(MV1DuplicateModel(m_ultGageUpHandle), pos);
		break;
	case ItemType::AttackUp:
		item = std::make_shared<AttackUp>(MV1DuplicateModel(m_attackUpHandle), pos);
		break;
	case ItemType::DefenseUp:
		item = std::make_shared<DefenseUp>(MV1DuplicateModel(m_defenseUpHandle), pos);
		break;
	default:
		break;
	}
	//アイテムを入れる
	AddNextActor(item);
	return item;
}

void ActorManager::AllDeleteNormalEnemy()
{
	//敵
	for (auto& actor : m_actors)
	{
		//敵を探す
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			auto enemy = std::dynamic_pointer_cast<EnemyBase>(actor);
			//ボス以外なら
			if (enemy->GetEnemyGrade() != EnemyGrade::Boss)
			{
				enemy->Delete();
			}
		}
	}
}

//プレイヤーに近い敵を取得
std::weak_ptr<Actor> ActorManager::GetNearestEnemy() const
{
	std::weak_ptr<Actor> nearestEnemy; //近い敵のポインタ
	float minDis = 1000000.0f; //初期値は大きな値
	for (auto& actor : m_actors)
	{
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			//プレイヤーに近い敵を探す
			float dis = (m_player.lock()->GetPos() - actor->GetPos()).Magnitude();
			if (dis < minDis)
			{
				minDis = dis;
				nearestEnemy = actor; //近い敵を更新
			}
		}
	}
	return nearestEnemy;
}

AttackData ActorManager::GetAttackData(std::string& ownerName, std::string& attackName)
{
	AttackData attackData;
	for (auto data : m_attackDatas)
	{
		//持ち主と攻撃の名前が同じなら
		if (data.ownerName == ownerName && data.attackName == attackName)
		{
			attackData = data;
			break;
		}
	}
	return attackData;
}

//アクターの消滅フラグをチェックして削除
void ActorManager::CheckDeleteActors(const std::weak_ptr<Score> score)
{
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		for (auto& actor : m_actors)
		{
			bool isDead = actor->IsDelete();//死亡したかをチェック
			if (isDead)
			{
				isOneMore = true;
				//死亡したアクターの終了処理
				actor->Dead(thisPointer, score);
				//削除候補
				deleteActer.emplace_back(actor);
			}
		}
		//削除
		for (auto& actor : deleteActer)
		{
			thisPointer->Exit(actor);
		}
		deleteActer.clear();
		if (!isOneMore)break;
	}
}

//追加予定のアクターを実装
void ActorManager::CheckNextAddActors()
{
	for (auto& actor : m_nextAddActors)
	{
		Entry(actor);
	}
	m_nextAddActors.clear();//追加予定のアクターは消す
}

//攻撃データの取得
void ActorManager::CreateAttackData()
{
	m_attackDatas = CSVDataLoader::LoadAttackDataCSV("Data/CSV/CharacterAttackData.csv");
}

void ActorManager::LoadHandle()
{
	m_playerHandle = MV1LoadModel("Data/Model/Player/Player.mv1");
	assert(m_playerHandle >= 0);
	m_wallHandle = MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1");
	assert(m_wallHandle >= 0);
	m_purpleDinosaurHandle = MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1");
	assert(m_purpleDinosaurHandle >= 0);
	m_smallDragonHandle = MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1");
	assert(m_smallDragonHandle >= 0);
	m_bomberHandle = MV1LoadModel("Data/Model/Enemy/Bomber.mv1");
	assert(m_bomberHandle >= 0);
	m_bossDragonHandle = MV1LoadModel("Data/Model/Enemy/BossDragon.mv1");
	assert(m_bossDragonHandle >= 0);
	m_pathHandle = MV1LoadModel("Data/Model/Stage/1/Path.mv1");
	assert(m_pathHandle >= 0);
	m_blockGrassHandle = MV1LoadModel("Data/Model/Stage/1/Block_Grass.mv1");
	assert(m_blockGrassHandle >= 0);
	m_cubeHandle = MV1LoadModel("Data/Model/Collision/Cube.mv1");
	assert(m_cubeHandle >= 0);
	m_cylinderHandle = MV1LoadModel("Data/Model/Collision/Cylinder.mv1");
	assert(m_cylinderHandle >= 0);
	m_planeHandle = MV1LoadModel("Data/Model/Collision/Plane.mv1");
	assert(m_planeHandle >= 0);
	m_skyHandle = MV1LoadModel("Data/Model/Stage/Sky/Sky_Daylight02.pmx");
	assert(m_skyHandle >= 0);
	m_heartHandle = MV1LoadModel("Data/Model/Item/Heart.mv1");
	assert(m_heartHandle >= 0);
	m_bombHandle = MV1LoadModel("Data/Model/Item/Bomb.mv1");
	assert(m_bombHandle >= 0);
	m_ultGageUpHandle = MV1LoadModel("Data/Model/Item/UltGageUp.mv1");
	assert(m_ultGageUpHandle >= 0);
	m_attackUpHandle = MV1LoadModel("Data/Model/Item/AttackUp.mv1");
	assert(m_attackUpHandle >= 0);
	m_defenseUpHandle = MV1LoadModel("Data/Model/Item/DefenseUp.mv1");
	assert(m_defenseUpHandle >= 0);
}
void ActorManager::LoadStage(Stage::StageIndex index)
{
	//配置データを取得
	std::string charaPath;
	std::string drawPath;
	std::string collPath;
	std::string bossAreaPath;
	switch (index)
	{
	case Stage::StageIndex::Stage1:
		charaPath = "Data/CSV/CharacterTransformData.csv";
		drawPath = "Data/CSV/StageTransformData.csv";
		collPath = "Data/CSV/StageCollisionTransformData.csv";
		bossAreaPath = "Data/CSV/BossTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	auto characterData = CSVDataLoader::LoadTransformDataCSV(charaPath.c_str());
	//名前からオブジェクトを配置していく
	for (auto& charaData : characterData)
	{
		if (charaData.name == "Player")
		{
			auto player = CreateCharacter(CharacterType::Player, charaData.pos).lock();
			player->GetModel()->SetScale(charaData.scale);
			player->GetModel()->SetRot(charaData.rot);
			m_player = std::dynamic_pointer_cast<Player>(player);
		}
		else if (charaData.name == "SmallDragon")
		{
			auto smallDragon = CreateCharacter(CharacterType::SmallDragon, charaData.pos).lock();
			smallDragon->GetModel()->SetScale(charaData.scale);
			smallDragon->GetModel()->SetRot(charaData.rot);
		}
		else if (charaData.name == "BossDragon")
		{
			auto bossDragon = CreateCharacter(CharacterType::BossDragon, charaData.pos).lock();
			bossDragon->GetModel()->SetScale(charaData.scale);
			bossDragon->GetModel()->SetRot(charaData.rot);
			m_boss = std::dynamic_pointer_cast<BossDragon>(bossDragon);

		}
		else if (charaData.name == "Bomber")
		{
			auto bomber = CreateCharacter(CharacterType::Bomber, charaData.pos).lock();
			bomber->GetModel()->SetScale(charaData.scale);
			bomber->GetModel()->SetRot(charaData.rot);
		}
		else if (charaData.name == "PurpleDinosaur")
		{
			auto purpleDinosaur = CreateCharacter(CharacterType::PurpleDinosaur, charaData.pos).lock();
			purpleDinosaur->GetModel()->SetScale(charaData.scale);
			purpleDinosaur->GetModel()->SetRot(charaData.rot);
		}
	}
	//空を作成
	m_actors.emplace_back(std::make_shared<Sky>(m_skyHandle));
	//描画用
	//配置データを取得
	auto stageDrawData = CSVDataLoader::LoadTransformDataCSV(drawPath.c_str());
	//名前からオブジェクトを配置していく
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> path =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_pathHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(path);
		}
		else if (stageData.name == "Block_Grass")
		{
			//大きさを1/100しないと大きすぎるので
			stageData.scale = VScale(stageData.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_blockGrassHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(blockGrass);
		}
	}
	//当たり判定用
	//配置データを取得
	auto stageCollData = CSVDataLoader::LoadTransformDataCSV(collPath.c_str());
	//名前からコリジョンを配置していく
	for (auto& stageData : stageCollData)
	{
		if (stageData.name == "Plane")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(plane);
		}
		else if (stageData.name == "BossAreaWall")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(plane);
		}
	}
	//ボス部屋を作成
	auto bossAreaData = CSVDataLoader::LoadTransformDataCSV(bossAreaPath.c_str());
	VECTOR startPos = {};
	VECTOR endPos = {};
	//名前からコリジョンを配置していく
	for (auto& stageData : bossAreaData)
	{
		if (stageData.name == "Start")
		{
			startPos = stageData.pos;
		}
		else if (stageData.name == "End")
		{
			endPos = stageData.pos;
		}
	}
	auto bossArea = std::make_shared<BossArea>(startPos, endPos);
	m_bossArea = bossArea;
	m_nextAddActors.emplace_back(bossArea);
}
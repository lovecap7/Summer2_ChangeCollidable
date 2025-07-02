#include "ActorManager.h"
#include "../StageSetup.h"
#include "Actor.h"
#include "Character/Player/Player.h"
#include "Character/Enemy/EnemyBase.h"
#include "Stage/BossArea.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"
#include "Attack/Slash.h"
#include "Attack/Strike.h"
#include "Attack/AreaOfEffectAttack.h"
#include "Attack/Bullet.h"
#include "Attack/Blast.h"
#include "Attack/Breath.h"
#include "Item/Heart.h"
#include "Item/UltGageUp.h"
#include "Item/Bomb.h"
#include "Item/AttackUp.h"
#include "Item/DefenseUp.h"
#include <DxLib.h>

ActorManager::ActorManager():
	m_actorId(0),
	m_heartHandle(-1),
	m_bombHandle(-1),
	m_ultGageUpHandle(-1),
	m_attackUpHandle(-1),
	m_defenseUpHandle(-1)
{
}

ActorManager::~ActorManager()
{
}

void ActorManager::Init(std::unique_ptr<StageSetup>& stageSetup)
{
	std::list<std::shared_ptr<Actor>> actors;
	//アクターを受け取る
	stageSetup->MoveActorsPointer(actors);
	//アクターの初期化処理
	for (auto& actor : actors)
	{
		//追加とIDの割り振り
		AddActor(actor);
	}
	actors.clear();//受け取ったアクターは消す
	//プレイヤーの参照
	for (auto& actor : m_actors)
	{
		//プレイヤーを探す
		if (actor->GetGameTag() == GameTag::Player)
		{
			m_player = std::dynamic_pointer_cast<Player>(actor);
			break;
		}
	}
	//ボスの参照
	for (auto& actor : m_actors)
	{
		//敵を探す
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			auto enemy = std::dynamic_pointer_cast<EnemyBase>(actor);
			//ボスなら
			if (enemy->GetEnemyGrade() == EnemyGrade::Boss)
			{
				m_boss = enemy;
				break;
			}
		}
	}
	//ボス部屋の参照
	for (auto& actor : m_actors)
	{
		//ボス部屋を探す
		if (actor->GetGameTag() == GameTag::Area)
		{
			m_bossArea = std::dynamic_pointer_cast<BossArea>(actor);
			break;
		}
	}
	//ハンドル
	m_heartHandle = MV1LoadModel("Data/Model/Item/Heart.mv1");
	m_bombHandle = MV1LoadModel("Data/Model/Item/Bomb.mv1");
	m_ultGageUpHandle = MV1LoadModel("Data/Model/Item/UltGageUp.mv1");
	m_attackUpHandle = MV1LoadModel("Data/Model/Item/AttackUp.mv1");
	m_defenseUpHandle = MV1LoadModel("Data/Model/Item/DefenseUp.mv1");
}

void ActorManager::Update(const std::weak_ptr<Camera> camera)
{
	//消滅フラグチェック
	CheckDeleteActors();
	//新規アクターの追加
	CheckNextAddActors();
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(camera,shared_from_this());
	}
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
	//アクターの終了処理
	for (auto& actor : m_actors)
	{
		actor->End();
	}
	//メモリを解放
	m_actors.clear();
	m_player.reset();
	//ハンドル
	MV1DeleteModel(m_heartHandle);
	MV1DeleteModel(m_bombHandle);
	MV1DeleteModel(m_ultGageUpHandle);
	MV1DeleteModel(m_attackUpHandle);
	MV1DeleteModel(m_defenseUpHandle);
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

//新規アクターの追加予定を受け取る(public)
void ActorManager::AddNextActor(std::shared_ptr<Actor> actor)
{
	//追加予定のアクターを追加
	m_nextAddActors.emplace_back(actor);
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

//アクターを追加
void ActorManager::AddActor(std::shared_ptr<Actor> actor)
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

//アクターの消滅フラグをチェックして削除
void ActorManager::CheckDeleteActors()
{
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [thisPointer, &isOneMore](std::shared_ptr<Actor> actor) {
			bool isDead = actor->IsDelete();//死亡したかをチェック
			if (isDead)
			{
				isOneMore = true;
				//死亡したアクターの終了処理
				actor->Dead(thisPointer);
				actor->End();
			}
			return isDead;
			});
		m_actors.erase(remIt, m_actors.end());//削除
		if (!isOneMore)break;
	}
}

//追加予定のアクターを実装
void ActorManager::CheckNextAddActors()
{
	for (auto& actor : m_nextAddActors)
	{
		AddActor(actor);
	}
	m_nextAddActors.clear();//追加予定のアクターは消す
}

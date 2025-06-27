#include "ActorManager.h"
#include "../StageSetup.h"
#include "Actor.h"
#include "Character/Player/Player.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"

ActorManager::ActorManager():
	m_actorId(0)
{
	//ステージを作成
	m_stageSetup = std::make_shared<StageSetup>();
}

ActorManager::~ActorManager()
{
}

void ActorManager::Init()
{
	//プレイヤーを受け取る
	m_stageSetup->MovePlayerPointer(m_player);
	std::list<std::shared_ptr<Actor>> actors;
	//アクターを受け取る
	m_stageSetup->MoveActorsPointer(actors);
	//アクターの初期化処理
	for (auto& actor : actors)
	{
		AddActor(actor);
	}
	actors.clear();//受け取ったアクターは消す
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
	//ステージセットアップの終了
	m_stageSetup->End();
	m_stageSetup.reset();
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
			float dis = (m_player->GetPos() - actor->GetPos()).Magnitude();
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
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> actor) {
		bool isDead = actor->IsDelete();//死亡したかをチェック
		if (isDead)
		{
			//死亡したアクターの終了処理
			actor->End();
		}
		return isDead;
		});
	m_actors.erase(remIt, m_actors.end());//削除
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

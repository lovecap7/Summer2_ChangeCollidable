#include "EventArea.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/Camera.h"

EventArea::EventArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	Actor(Shape::None),
	m_start(start),
	m_end(end)
{
}

EventArea::~EventArea()
{
}

void EventArea::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera,actorManager);
}

void EventArea::End()
{
}

void EventArea::EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	//座標から範囲に入ったかをチェック
	auto playerPos = player->GetPos();
	auto startPos = m_start.lock()->GetPos();
	auto endPos = m_end.lock()->GetPos();
	//範囲内なら
	if (playerPos.x > startPos.x && playerPos.x < endPos.x)
	{
		//範囲内のCollidableの参照を取得
		auto collList = Physics::GetInstance().GetAreaXCollidable(startPos.x, endPos.x);
		for (auto& coll : collList)
		{
			//範囲内の敵の参照を取得
			if (coll.expired())continue;
			if (coll.lock()->GetGameTag() == GameTag::Enemy)
			{
				//敵
				m_areaEnemies.emplace_back(std::dynamic_pointer_cast<EnemyBase>(coll.lock()));
			}
		}
		//イベント開始情報をカメラに設定
		camera.lock()->EventStart(startPos.x, endPos.x);
		m_update = &EventArea::EventUpdate;
		return;
	}
}

void EventArea::EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//範囲内の敵がすべて死亡した場合イベントを終了
	for (auto coll : m_areaEnemies)
	{
		//範囲内の敵がまだ生きているなら
		if (!coll.expired())return;
	}
	//範囲内の敵がすべて消えたら
	//カメラにイベントが終了したことを設定
	camera.lock()->EventEnd();
	//削除
	m_isDelete = true;
	//壁も削除
	m_start.lock()->Delete();
	m_end.lock()->Delete();
}

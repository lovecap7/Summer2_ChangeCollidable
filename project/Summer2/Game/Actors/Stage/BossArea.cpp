#include "BossArea.h"
#include "StageObjectCollision.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/GameCamera/GameCamera.h"
BossArea::BossArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	EventAreaBase(start,end,AreaTag::Boss),
	m_update(&BossArea::EntryCheckUpdate)
{
}

BossArea::~BossArea()
{
}

void BossArea::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera, actorManager);
}

void BossArea::EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	EventAreaBase::Update(camera, actorManager);
	if (m_isEvent)
	{
		//壁は閉ざす
		std::dynamic_pointer_cast<StageObjectCollision>(m_start.lock())->SetIsThrough(false);
		std::dynamic_pointer_cast<StageObjectCollision>(m_end.lock())->SetIsThrough(false);
		//イベント開始情報をカメラに設定
		camera.lock()->SetEventArea(std::dynamic_pointer_cast<BossArea>(shared_from_this()));
		//ボス以外の雑魚敵を削除
		actorManager.lock()->AllDeleteNormalEnemy();
		m_update = &BossArea::EventUpdate;
		return;
	}
}

void BossArea::EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが倒れたら
	if (actorManager.lock()->GetBoss().expired())
	{
		//範囲内の敵がすべて消えたら
		//イベント終了
		m_isEvent = false;
		//このエリアも消す
		m_isDelete = true;
	}
}

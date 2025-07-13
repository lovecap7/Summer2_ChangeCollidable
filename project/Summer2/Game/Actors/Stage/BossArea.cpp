#include "BossArea.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/Camera.h"
BossArea::BossArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	EventArea(start,end),
	m_isEntryBossArea(false),
	m_update(&BossArea::EntryCheckUpdate)
{
}

BossArea::~BossArea()
{
}

void BossArea::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera, actorManager);
}

void BossArea::End()
{
}

void BossArea::EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		//イベント開始情報をカメラに設定
		camera.lock()->SetEventArea(std::dynamic_pointer_cast<EventArea>(shared_from_this()));
		//イベントフラグ
		m_isEvent = true;
		//ボスエリアに入ったフラグ
		m_isEntryBossArea = true;
		//ボス以外の雑魚敵を削除
		actorManager.lock()->AllDeleteNormalEnemy();
		m_update = &BossArea::EventUpdate;
		return;
	}
}

void BossArea::EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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

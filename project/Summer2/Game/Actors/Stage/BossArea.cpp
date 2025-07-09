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
		camera.lock()->EventStart(startPos.x, endPos.x);
		//ボスエリアに入ったフラグ
		m_isEntryBossArea = true;
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
		//カメラにイベントが終了したことを設定
		camera.lock()->EventEnd();
	}
}

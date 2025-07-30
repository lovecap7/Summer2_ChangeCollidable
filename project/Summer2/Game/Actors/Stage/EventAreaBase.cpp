#include "EventAreaBase.h"
#include "StageObjectCollision.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/GameCamera/GameCamera.h"
EventAreaBase::EventAreaBase(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	Actor(Shape::None),
	m_start(start),
	m_end(end),
	m_isEvent(false)
{
}

EventAreaBase::~EventAreaBase()
{
}

void EventAreaBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (m_isEvent)return; //イベント中なら更新しない

	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	//座標から範囲に入ったかをチェック
	auto playerPos = player->GetPos();
	auto startPos = m_start.lock()->GetPos();
	auto endPos = m_end.lock()->GetPos();

	//範囲内にいるか
	Vector2 sToEVec = endPos.XZ() - startPos.XZ();
	Vector2 sToPVec = playerPos.XZ() - startPos.XZ();
	if (sToEVec.Magnitude() > 0.0f && sToPVec.Magnitude())
	{
		sToEVec = sToEVec.Normalize();
		sToPVec = sToPVec.Normalize();
	}
	//内積の結果が0以上かつ1以下なら範囲内
	float dot = sToEVec.Dot(sToPVec);
	dot = MathSub::ClampFloat(dot, -1.0f, 1.0f);
	if (dot >= 0.0f && dot <= 1.0f)
	{
		m_isEvent = true; //イベント中フラグを立てる
	}
	//壁はすり抜ける
	std::dynamic_pointer_cast<StageObjectCollision>(m_start.lock())->SetIsThrough(true);
	std::dynamic_pointer_cast<StageObjectCollision>(m_end.lock())->SetIsThrough(true);
}

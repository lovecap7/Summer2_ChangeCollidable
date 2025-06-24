#include "SearchTrigger.h"
#include "SphereCollider.h"
#include "../Rigidbody.h"
#include "../../Game/Actors/Actor.h"
#include "../Collidable.h"

SearchTrigger::SearchTrigger(float radius, float viewAngle, std::shared_ptr<Actor> owner):
	m_viewAngle(viewAngle),
	m_isHit(false),
	m_target(Vector3::Zero())
{
	m_serchTrigger = std::make_shared<Collidable>(std::make_shared<SphereCollider>(radius), owner->GetCollidable()->GetRb());
}

SearchTrigger::SearchTrigger(float radius, std::shared_ptr<Actor> owner):
	m_viewAngle(360.0f * MyMath::DEG_2_RAD),
	m_isHit(false),
	m_target(Vector3::Zero())
{
	m_serchTrigger = std::make_shared<Collidable>(std::make_shared<SphereCollider>(radius), owner->GetCollidable()->GetRb());
}

SearchTrigger::~SearchTrigger()
{
}

void SearchTrigger::Init()
{
	m_isHit = false;
}

void SearchTrigger::SetPos(Vector3 pos)
{
	m_serchTrigger->GetRb()->SetPos(pos);
}

void SearchTrigger::OnHitTarget(Vector3 target)
{
	m_isHit = true;
	m_target = target;
}

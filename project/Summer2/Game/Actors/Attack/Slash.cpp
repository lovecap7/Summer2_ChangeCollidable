#include "Slash.h"

Slash::Slash(int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, std::weak_ptr<ActorStateBase> ownerState) :
	AttackBase(Shape::Capsule,damage, keepFrame, knockBackPower, aw, ownerState)
{
}

Slash::~Slash()
{
}

void Slash::Init()
{
}

void Slash::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
}

void Slash::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Slash::Draw() const
{
}

void Slash::Complete()
{
}

Vector3 Slash::GetKnockBackVec(Vector3 other)
{
	return Vector3();
}

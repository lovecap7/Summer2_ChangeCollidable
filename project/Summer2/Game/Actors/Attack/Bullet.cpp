#include "Bullet.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../Character/CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/UltGage.h"
Bullet::Bullet(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner)
{
}

void Bullet::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//‚¿å‚ª•sİ‚È‚ç
	if (m_owner.expired())
	{
		m_isDelete = true;
		return;
	}

	//ˆÚ“®
	m_rb->m_pos = m_rb->GetNextPos();
	//‹¤’Ê‚Ìˆ—‚ğ‚·‚é
	AttackBase::Update();
}

void Bullet::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//©•ª‚Æ“¯‚¶í—Ş‚È‚ç–³‹
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//ƒvƒŒƒCƒ„[‚©“G‚È‚ç
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints().lock()->IsNoDamege())
		{
			//ƒ_ƒ[ƒW‚ğó‚¯‚È‚¢ó‘Ô‚È‚ç–³‹
			return;
		}
	}
	else
	{
		//‚»‚êˆÈŠO‚ÌActor‚Í–³‹
		return;
	}
	//ƒvƒŒƒCƒ„[‚ÌUŒ‚‚Ìê‡•KE‹ZƒQ[ƒW‚ğ‰ÁZ‚·‚é
	if (ownerColl->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage()->AddPedingUltGage();//—\–ñ‚³‚ê‚Ä‚¢‚½‰ÁZƒQ[ƒW—Ê‚ğ”½‰f
	}
	//UŒ‚‚ğó‚¯‚½‚Æ‚«‚Ìˆ—
	std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	//Á–Å
	m_isDelete = true;
}

void Bullet::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}


void Bullet::SetVec(Vector3 vec)
{
	//ˆÚ“®—Ê
	m_rb->m_vec = vec;
}
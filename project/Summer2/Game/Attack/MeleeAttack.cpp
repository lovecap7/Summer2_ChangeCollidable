#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Rigidbody.h"
#include "../Actors/Player/Player.h"
#include "../Actors/Player/UltGage.h"

MeleeAttack::MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner) :
	AttackBase(coll, damage, keepFrame, knockBackPower, owner)
{
}

MeleeAttack::~MeleeAttack()
{
}

void MeleeAttack::Init()
{
	//‰Šú‰»ˆ—
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDelete = false;
	//ID‚Ì‰Šú‰»
	if (m_hitId.size() != 0)
	{
		m_hitId.clear();
	}
}

void MeleeAttack::Update()
{
	//‘±ƒtƒŒ[ƒ€‚ğŒ¸‚ç‚·
	m_keepFrame--;
	//‘±ƒtƒŒ[ƒ€‚ª0‚É‚È‚Á‚½‚çÁ–Å
	if (m_keepFrame <= 0)
	{
		m_isDelete = true;
	}
}

void MeleeAttack::Draw()
{
#if _DEBUG
	if (m_collidable->GetColl()->GetShape() == Shape::Capsule)
	{
		DrawCapsule3D(
			m_collidable->GetRb()->GetPos().ToDxLibVector(),
			std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
			std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
			32,
			0xff00ff,
			0xff00ff,
			false
		);
	}
	else if(m_collidable->GetColl()->GetShape() == Shape::Sphere)
	{
		DrawSphere3D(
			m_collidable->GetRb()->GetPos().ToDxLibVector(),
			std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
			32,
			0xff00ff,
			0xff00ff,
			false
		);
	}
	
#endif
}

void MeleeAttack::OnHit(std::shared_ptr<Actor> actor)
{
	auto ownerColl = m_owner->GetCollidable();
	auto otherColl = actor->GetCollidable();

	//©•ª‚Æ“¯‚¶í—Ş‚È‚ç–³‹
	if (ownerColl->GetGameTag() == otherColl->GetGameTag())return;

	bool isFind = false;
	//ID‚ª‚·‚Å‚É‹L˜^‚³‚ê‚Ä‚¢‚é‚©Šm”F
	for (auto id : m_hitId)
	{
		if (id == actor->GetID())
		{
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		//‹L˜^‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î‹L˜^‚·‚é
		m_hitId.emplace_back(actor->GetID());
		//UŒ‚‚ğ‹ò‚ç‚Á‚½‚Ìˆ—‚ğ‚·‚é
		auto hurtPoint = actor->GetHurtPoint();
		hurtPoint->OnHit(shared_from_this());
		//ƒvƒŒƒCƒ„[‚ÌUŒ‚‚Ìê‡•KE‹ZƒQ[ƒW‚ğ‰ÁZ‚·‚é
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			auto gage = std::dynamic_pointer_cast<Player>(m_owner)->GetUltGage();
			gage->AddPedingUltGage();//—\–ñ‚³‚ê‚Ä‚¢‚½‰ÁZƒQ[ƒW—Ê‚ğ”½‰f
		}
	}
}

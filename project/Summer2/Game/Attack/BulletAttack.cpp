#include "BulletAttack.h"
#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"

BulletAttack::BulletAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame,float knockBackPower, std::shared_ptr<Actor> owner):
	AttackBase(coll, damage,keepFrame, knockBackPower, owner),
	m_dir(Vector3::Zero()),
	m_speed(0.0f)
{
}

BulletAttack::~BulletAttack()
{
}

void BulletAttack::Init()
{
	//初期化処理
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDelete = false;
	//IDの初期化
	if (m_hitId.size() != 0)
	{
		m_hitId.clear();
	}
}

void BulletAttack::Update()
{
	//移動
	if (m_dir.Magnitude() > 0.0f)
	{
		Vector3 moveVec = m_dir.Normalize() * m_speed;
		m_collidable->GetRb()->SetPos(m_collidable->GetRb()->GetPos() + moveVec);
	}

	//持続フレームを減らす
	m_keepFrame--;
	//持続フレームが0になったら消滅
	if (m_keepFrame <= 0)
	{
		m_isDelete = true;
	}
}

void BulletAttack::Draw()
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
	else if (m_collidable->GetColl()->GetShape() == Shape::Sphere)
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

void BulletAttack::OnHit(std::shared_ptr<Actor> actor)
{
	auto ownerColl = m_owner->GetCollidable();
	auto otherColl = actor->GetCollidable();
	//自分と同じ種類なら無視
	if (ownerColl->GetGameTag() == otherColl->GetGameTag())return;

	bool isFind = false;
	//IDがすでに記録されているか確認
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
		//記録されていなければ記録する
		m_hitId.emplace_back(actor->GetID());
		//攻撃を喰らった時の処理をする
		auto hurtPoint = actor->GetHurtPoint();
		hurtPoint->OnHit(shared_from_this());
		//削除
		m_isDelete = true;
	}
}

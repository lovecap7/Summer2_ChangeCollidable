#include "BlastAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Rigidbody.h"

BlastAttack::BlastAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner) :
	AttackBase(coll, damage, keepFrame, knockBackPower, owner)
{
	Init();
}

BlastAttack::~BlastAttack()
{
}

void BlastAttack::Init()
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

void BlastAttack::Update()
{
	//持続フレームを減らす
	m_keepFrame--;
	//持続フレームが0になったら消滅
	if (m_keepFrame <= 0)
	{
		m_isDelete = true;
	}
}

void BlastAttack::Draw()
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

void BlastAttack::OnHit(std::shared_ptr<Actor> actor)
{
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
	}
}

Vector3 BlastAttack::GetKnockBackVec(Vector3 other)
{
	//ノックバック
	Vector3 knockBackVec = other - m_collidable->GetRb()->GetPos();//離れるベクトル
	knockBackVec.y = 0.0f;//Y成分はなし
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//ノックバック
	return knockBackVec;
}

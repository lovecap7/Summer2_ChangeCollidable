#include "Blast.h"
#include "AreaOfEffectAttack.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"

Blast::Blast(std::weak_ptr<Actor> owner) :
	SphereAttackBase(owner)
{
}

Blast::~Blast()
{
}

void Blast::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//持続フレームを減らす
	--m_keepFrame;
	//持続フレームが0になったら削除
	if (m_keepFrame < 0)
	{
		m_isDelete = true;	//削除フラグを立てる
		m_isThrough = true;	//当たり判定をしない
		return; //何もしない
	}
}

void Blast::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}

Vector3 Blast::GetKnockBackVec(Position3 other)
{
	//ノックバック
	Vector3 knockBackVec = other - m_rb->GetPos();
	knockBackVec.y = 0.0f;//Y成分はなし
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//ノックバック
	return knockBackVec;
}



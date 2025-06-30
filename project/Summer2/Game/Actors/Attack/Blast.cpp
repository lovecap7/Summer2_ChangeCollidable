#include "Blast.h"
#include "AreaOfEffectAttack.h"
#include "../Character/CharacterBase.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/HitPoints.h"

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

void Blast::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto otherColl = other;
	//プレイヤーか敵なら
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints().lock()->IsNoDamege())
		{
			//ダメージを受けない状態なら無視
			return;
		}
	}
	else
	{
		//それ以外のActorは無視
		return;
	}
	std::shared_ptr<Actor> otherActor = std::dynamic_pointer_cast<Actor>(otherColl);
	bool isFind = false;
	//IDがすでに記録されているか確認
	for (auto id : m_hitId)
	{
		if (id == otherActor->GetID())
		{
			isFind = true;
			break;
		}
	}
	//攻撃成功
	if (!isFind)
	{
		//記録されていなければ記録する
		m_hitId.emplace_back(otherActor->GetID());
		//攻撃を受けたときの処理
		std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
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



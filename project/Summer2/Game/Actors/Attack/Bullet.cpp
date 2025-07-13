#include "Bullet.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Effect/EffekseerManager.h"
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
	//持ち主が不在なら
	if (m_owner.expired())
	{
		m_isDelete = true;
		return;
	}

	//移動
	m_rb->m_pos = m_rb->GetNextPos();
	//共通の処理をする
	AttackBase::Update();
}

void Bullet::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_owner.expired())return;
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//自分と同じ種類なら無視
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

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
	//プレイヤーの攻撃の場合必殺技ゲージを加算する
	if (ownerColl->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage().lock()->AddPedingUltGage();//予約されていた加算ゲージ量を反映
	}
	//攻撃を受けたときの処理
	std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	//消滅
	m_isDelete = true;
	//ヒットエフェクト
	auto HitPos = m_rb->m_pos;
	EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", HitPos);
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
	//移動量
	m_rb->m_vec = vec;
}

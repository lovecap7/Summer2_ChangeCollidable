#include "AttackBase.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyBase.h"
#include "../ActorStateBase.h"
#include "../Player/UltGage.h"
#include "../../../General/HitPoints.h"

AttackBase::AttackBase(Shape shape, std::weak_ptr<ActorStateBase> ownerState):
	Actor(shape),
	m_damage(0.0f),
	m_keepFrame(0.0f),
	m_knockBackPower(0.0f),
	m_attackWeight(Battle::AttackWeight::Light),
	m_ownerState(ownerState)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto ownerColl = m_ownerState.lock()->GetOwner().lock();
	auto otherColl = other;

	//自分と同じ種類なら無視
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//プレイヤーか敵なら
	if (otherColl->GetGameTag() == GameTag::Player)
	{
		if (std::dynamic_pointer_cast<Player>(otherColl)->GetHitPoints()->IsNoDamege())
		{
			//ダメージを受けない状態なら無視
			return;
		}
	}
	else if (otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<EnemyBase>(otherColl)->GetHitPoints()->IsNoDamege())
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
	if (!isFind)
	{
		//記録されていなければ記録する
		m_hitId.emplace_back(otherActor->GetID());
		//プレイヤーの攻撃の場合必殺技ゲージを加算する
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			auto gage = std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage();
			gage->AddPedingUltGage();//予約されていた加算ゲージ量を反映
		}
	}
}

Vector3 AttackBase::GetKnockBackVec(Position3 other)
{
	//ノックバック
	Vector3 knockBackVec = other - m_ownerState.lock()->GetOwner().lock()->GetNextPos();
	knockBackVec.y = 0.0f;//Y成分はなし
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//ノックバック
	return knockBackVec;
}

void AttackBase::AttackSetting(int damage, int keepFrame, int knockBackPower, Battle::AttackWeight aw)
{
	m_damage = damage;
	m_keepFrame = keepFrame;
	m_knockBackPower = knockBackPower;
	m_attackWeight = aw;
}

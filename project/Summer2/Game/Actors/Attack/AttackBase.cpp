#include "AttackBase.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Character/CharacterStateBase.h"
#include "../Character/Player/UltGage.h"
#include "../../../General/HitPoints.h"

AttackBase::AttackBase(Shape shape, std::weak_ptr<Actor> owner):
	Actor(shape),
	m_owner(owner),
	m_damage(0.0f),
	m_keepFrame(0.0f),
	m_knockBackPower(0.0f),
	m_attackWeight(Battle::AttackWeight::Light),
	m_ownerTag(owner.lock()->GetGameTag())
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init()
{
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Attack, false, true,false);
	Collidable::Init();
}

void AttackBase::Update()
{
	if (m_owner.expired())
	{
		m_isDelete = true; //所有者のステートが無くなったら削除フラグを立てる
		return; //所有者のステートが無くなったら何もしない
	}
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

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//自分と同じ種類なら無視
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//プレイヤーか敵なら
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints()->IsNoDamege())
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
		//プレイヤーの攻撃の場合必殺技ゲージを加算する
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			std::dynamic_pointer_cast<Player>(m_owner.lock())->GetUltGage()->AddPedingUltGage();//予約されていた加算ゲージ量を反映
		}
		//攻撃を受けたときの処理
		std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	}
}

Vector3 AttackBase::GetKnockBackVec(Position3 other)
{
	//ノックバック
	Vector3 knockBackVec = other - m_owner.lock()->GetNextPos();
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

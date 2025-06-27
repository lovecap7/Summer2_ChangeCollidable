#include "PlayerStateChargeAttackBase.h"
#include "../../Attack/Strike.h"
#include "../../../../General/Model.h"
#include "../../ActorManager.h"

namespace
{
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
}

PlayerStateChargeAttackBase::PlayerStateChargeAttackBase(std::weak_ptr<Actor> player):
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
}

PlayerStateChargeAttackBase::~PlayerStateChargeAttackBase()
{
}

void PlayerStateChargeAttackBase::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//攻撃を作成
	auto attack = std::make_shared<Strike>(m_owner);
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, Battle::AttackWeight::Middle);
	//参照
	m_attack = attack;
	//攻撃を入れる
	actorManager.lock()->AddNextActor(attack);
}

void PlayerStateChargeAttackBase::UpdateStrikeAttackPos(std::weak_ptr<Strike> attack)
{
	//左足の状態を更新したら攻撃も更新される
	auto model = m_owner.lock()->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	//座標をセット
	attack.lock()->SetStartPos(root);
	attack.lock()->SetEndPos(toe);
}

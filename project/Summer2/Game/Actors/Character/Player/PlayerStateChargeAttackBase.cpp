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
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Strike>(actorManager.lock()->CreateAttack(AttackType::Strike, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}

void PlayerStateChargeAttackBase::UpdateAttackPos()
{
	//左足の状態を更新したら攻撃も更新される
	auto model = m_owner.lock()->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	//座標をセット
	m_attack.lock()->SetStartPos(root);
	m_attack.lock()->SetEndPos(toe);
}

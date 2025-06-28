#include "PlayerStateAttackNormalBase.h"
#include "../../Attack/Slash.h"
#include "../../../../General/Model.h"
#include "../../ActorManager.h"
#include "../../../../General/Rigidbody.h"
#include "Player.h"
namespace
{
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さと半径
	constexpr float kSwordHeight = 150.0f;
}

PlayerStateAttackNormalBase::PlayerStateAttackNormalBase(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
}

PlayerStateAttackNormalBase::~PlayerStateAttackNormalBase()
{
}
void PlayerStateAttackNormalBase::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}
void PlayerStateAttackNormalBase::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	//座標をセット
	m_attack.lock()->SetStartPos(ringFinger);
	m_attack.lock()->SetEndPos(swordDir);
}
void PlayerStateAttackNormalBase::AttackMove(float speed)
{
	//ターゲットを索敵してるなら(ターゲットのほうを向く)
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//向き
	Vector3 dir = coll->GetStickVec().XZ();
	//ターゲットが発見できた時
	if (targetData.isHitTarget)
	{
		dir = targetData.targetDirXZ;
	}
	//向きの更新
	coll->GetModel()->SetDir(dir.XZ());
	//向いてる方向に移動
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * speed);
}

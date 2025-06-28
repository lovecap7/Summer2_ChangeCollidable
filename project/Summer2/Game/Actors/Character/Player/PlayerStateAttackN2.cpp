#include "PlayerStateAttackN2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttackN3.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "UltGage.h"
#include "../../Attack/Slash.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//通常攻撃2のダメージと持続フレーム
	constexpr int kAN2Damege = 200.0f;
	constexpr int kAN2KeepFrame = 6;
	constexpr int kAN2StartFrame = 15;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//アニメーション
	const char* kAnim = "Player|NA2";
	//弱攻撃の段階別アニメーションの速度
	constexpr float kAN2AnimSpeed = 1.3f;
	//攻撃終了前にキャンセル可能フレーム
	constexpr float kAttackCancelFrame = 20.0f;
	//武器の長さと半径
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//加算ゲージ量
	constexpr int kAddUltGage = 2;
	//移動フレーム
	constexpr int kMoveFrame = 5;
	//移動量
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN2::PlayerStateAttackN2(std::weak_ptr<Actor> player) :
	PlayerStateAttackNormalBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//通常攻撃2
	coll->SetCollState(CollisionState::Normal);
	//攻撃2
	coll->GetModel()->SetAnim(kAnim, false, kAN2AnimSpeed);
}

PlayerStateAttackN2::~PlayerStateAttackN2()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage()->SetPendingUltGage(0);
}
void PlayerStateAttackN2::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateAttackN2::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//死亡したなら
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//攻撃を受けたなら
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//カウント
	++m_attackCountFrame;
	auto model = coll->GetModel();
	//攻撃発生フレーム
	if (m_attackCountFrame == kAN2StartFrame)
	{
		CreateAttack(kRightSwordRadius, kAN2Damege, kAN2KeepFrame, kKnockBackPower, Battle::AttackWeight::Middle, actorManager);
	}
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("A"))
		{
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
			return;
		}
		//3段目
		if (input.IsTrigger("X"))
		{
			ChangeState(std::make_shared<PlayerStateAttackN3>(m_owner));
			return;
		}
		//チャージボタンを押したら
		if (input.IsTrigger("Y"))
		{
			//チャージ
			ChangeState(std::make_shared<PlayerStateCharge>(m_owner));
			return;
		}
	}

	//攻撃の位置更新
	if (!m_attack.expired())
	{
		UpdateAttackPos();
	}
	//移動フレーム中は前に進む
	if (m_attackCountFrame <= kMoveFrame)
	{
		AttackMove(kMoveSpeed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
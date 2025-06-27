#include "PlayerStateAttackN1.h"
#include "PlayerStateAttackN2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
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
	//通常攻撃1のダメージと持続フレームと発生フレーム
	constexpr int kAN1Damege = 100.0f;
	constexpr int kAN1KeepFrame = 6;
	constexpr int kAN1StartFrame = 12;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//アニメーション
	const char* kAnim = "Player|NA1";
	//弱攻撃の段階別アニメーションの速度
	constexpr float kAN1AnimSpeed = 1.3f;
	//攻撃終了前にキャンセル可能フレーム
	constexpr float kAttackCancelFrame = 20.0f;
	//攻撃の半径
	constexpr float kRightSwordRadius = 20.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//攻撃がヒットしたときの加算ゲージ量
	constexpr int kAddUltGage = 1;
	//移動フレーム
	constexpr int kMoveFrame = 5;
	//移動量
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN1::PlayerStateAttackN1(std::weak_ptr<Actor> player):
	PlayerStateAttackNormalBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//通常攻撃1
	coll->SetCollState(CollisionState::Normal);
	//攻撃1
	coll->GetModel()->SetAnim(kAnim, false, kAN1AnimSpeed);
	//加算ゲージの予約
	coll->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateAttackN1::~PlayerStateAttackN1()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage()->SetPendingUltGage(0);
}

void PlayerStateAttackN1::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateAttackN1::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	if (m_attackCountFrame == kAN1StartFrame)
	{
		CreateAttack(kRightSwordRadius,kAN1Damege,kAN1KeepFrame,kKnockBackPower,Battle::AttackWeight::Middle, actorManager);
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
		//2段目
		if (input.IsTrigger("X"))
		{
			ChangeState(std::make_shared<PlayerStateAttackN2>(m_owner));
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
		UpdateSlashAttackPos(m_attack);
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}


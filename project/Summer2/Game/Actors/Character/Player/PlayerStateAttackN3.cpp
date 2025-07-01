#include "PlayerStateAttackN3.h"
#include "PlayerStateIdle.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateHit.h"
#include "PlayerStateUltimate.h"
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
	//通常攻撃3のダメージと持続フレーム
	constexpr int kAN3Damege = 300.0f;
	constexpr int kAN3KeepFrame = 13;
	constexpr int kAN3StartFrame = 15;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 4.0f;
	//アニメーション
	const char* kAnim = "Player|NA3";//通常攻撃3
	//弱攻撃の段階別アニメーションの速度
	constexpr float kAN3AnimSpeed = 1.3f;
	//攻撃終了前にキャンセル可能フレーム
	constexpr float kAttackCancelFrame = 30.0f;
	//半径
	constexpr float kRightSwordRadius = 10.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//加算ゲージ量
	constexpr int kAddUltGage = 3;
	//移動フレーム
	constexpr int kMoveFrame = 10;
	//移動量
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN3::PlayerStateAttackN3(std::weak_ptr<Actor> player) :
	PlayerStateAttackNormalBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//通常攻撃2
	coll->SetCollState(CollisionState::Move);
	//攻撃2
	coll->GetModel()->SetAnim(kAnim, false, kAN3AnimSpeed);
}

PlayerStateAttackN3::~PlayerStateAttackN3()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
}
void PlayerStateAttackN3::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateAttackN3::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//死亡したなら
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//攻撃を受けたなら
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAN3StartFrame)
	{
		CreateAttack(kRightSwordRadius, kAN3Damege, kAN3KeepFrame, kKnockBackPower, Battle::AttackWeight::Middle, actorManager);
	}
	auto model = coll->GetModel();
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら3段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("A"))
		{
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
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
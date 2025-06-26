#include "PlayerStateAttackN1.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//通常攻撃1のダメージと持続フレームと発生フレーム
	constexpr int kAN1Damege = 100.0f;
	constexpr int kAN1KeepFrame = 4;
	constexpr int kAN1StartFrame = 15;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//アニメーション
	const char* kAnim = "Player|NA1";
	//弱攻撃の段階別アニメーションの速度
	constexpr float kAN1AnimSpeed = 1.3f;
	//攻撃終了前にキャンセル可能フレーム
	constexpr float kAttackCancelFrame = 20.0f;
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さと半径
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//攻撃がヒットしたときの加算ゲージ量
	constexpr int kAddUltGage = 1;
	//移動フレーム
	constexpr int kMoveFrame = 5;
	//移動量
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN1::PlayerStateAttackN1(std::weak_ptr<Player> player):
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	auto coll = m_player.lock();
	//通常攻撃1
	coll->SetCollState(CollisionState::Normal);
	//攻撃1
	coll->GetModel()->SetAnim(kAnim, false, kAN1AnimSpeed);
	//加算ゲージの予約
	coll->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateAttackN1::~PlayerStateAttackN1()
{
}

void PlayerStateAttackN1::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateAttackN1::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAN1StartFrame)
	{
		//攻撃を入れる
		
	}
	auto coll = m_player.lock();
	auto model = coll->GetModel();
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("A"))
		{
			//削除
			
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//2段目
		if (input.IsTrigger("X"))
		{
			//削除
			
			//ChangeState(std::make_shared<PlayerStateAttackN2>(m_player));
			return;
		}
		//チャージボタンを押したら
		if (input.IsTrigger("Y"))
		{
			//削除
			
			//チャージ
			//ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
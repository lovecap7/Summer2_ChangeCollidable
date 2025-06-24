#include "PlayerStateAttackN2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttackN3.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/HurtPoint.h"
#include "../../../General/Collision/SearchTrigger.h"
#include "../ActorManager.h"
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
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
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

PlayerStateAttackN2::PlayerStateAttackN2(std::shared_ptr<Player> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	//通常攻撃2
	m_player->GetCollidable()->SetState(State::None);
	//攻撃2
	m_player->GetModel()->SetAnim(kAnim, false, kAN2AnimSpeed);
	//攻撃判定の準備
	CreateAttack();
	//加算ゲージの予約
	m_player->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateAttackN2::~PlayerStateAttackN2()
{
	//加算ゲージの予約リセット
	m_player->GetUltGage()->SetPendingUltGage(0);
}
void PlayerStateAttackN2::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateAttackN2::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//攻撃マネージャー
	auto attackManager = actorManager->GetAttackManager();
	//死亡
	if (m_player->GetHurtPoint()->IsDead())
	{
		//削除
		DeleteAttack(attackManager);
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//攻撃を受けた時
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//削除
		DeleteAttack(attackManager);
		//やられ状態
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	//ゲージがあるとき使える
	if (input.IsTrigger("RB") && m_player->GetUltGage()->IsMaxUlt())
	{
		//削除
		DeleteAttack(attackManager);
		//必殺技
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player, actorManager));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAN2StartFrame)
	{
		//攻撃を入れる
		AppearAttack(m_attackN2, attackManager);
	}
	auto model = m_player->GetModel();
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//削除
		DeleteAttack(attackManager);
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら3段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("A"))
		{
			//削除
			DeleteAttack(attackManager);
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//3段目
		if (input.IsTrigger("X"))
		{
			//削除
			DeleteAttack(attackManager);
			ChangeState(std::make_shared<PlayerStateAttackN3>(m_player));
			return;
		}
		//チャージボタンを押したら
		if (input.IsTrigger("Y"))
		{
			//削除
			DeleteAttack(attackManager);
			//チャージ
			ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//攻撃の位置更新
	UpdateAttack();
	//少しずつ減速する
	SpeedDown();
	//攻撃時に前進する
	AttackMove();
}

void PlayerStateAttackN2::CreateAttack()
{
	auto model = m_player->GetModel();
	//武器の位置を出す
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kRightSwordRadius),
		std::make_shared<Rigidbody>(Vector3(ringFinger.x, ringFinger.y, ringFinger.z)));

	//武器の座標と当たり判定を攻撃に紐図ける
	m_attackN2 = std::make_shared<MeleeAttack>(m_rightSword, kAN2Damege, kAN2KeepFrame, kKnockBackPower, m_player);
}

void PlayerStateAttackN2::UpdateAttack()
{
	//武器の状態を更新したら攻撃も更新される

	auto model = m_player->GetModel();
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	//武器の持ち手をセット
	m_rightSword->GetRb()->SetPos(Position3(ringFinger.x, ringFinger.y, ringFinger.z));
	//武器の剣先をセット
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void PlayerStateAttackN2::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
void PlayerStateAttackN2::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃判定を消す
	m_attackN2->Delete();
	attackManager->Exit(m_attackN2);
}

void PlayerStateAttackN2::AttackMove()
{
	//移動フレーム中は前に進む
	if (m_attackCountFrame <= kMoveFrame)
	{
		//ターゲットを索敵してるなら(ターゲットのほうを向く)
		auto trigger = m_player->GetSearchTrigger();
		//向き
		Vector3 dir = m_player->GetStickVec().XZ();
		//ターゲットが発見できた時
		if (trigger->IsTargetHit())
		{
			auto pToT = trigger->GetTargetPos() - m_player->GetCollidable()->GetRb()->GetPos();
			dir = pToT;
		}
		//向きの更新
		m_player->GetModel()->SetDir(dir.XZ());
		//向いてる方向に移動
		m_player->GetCollidable()->GetRb()->SetMoveVec(m_player->GetModel()->GetDir() * kMoveSpeed);
	}
}

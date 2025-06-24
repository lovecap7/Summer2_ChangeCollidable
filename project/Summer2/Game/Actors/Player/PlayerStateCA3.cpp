#include "PlayerStateCA3.h"
#include "PlayerStateIdle.h"
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
#include "../ActorManager.h"
namespace
{
	//チャージ攻撃の段階別ダメージ
	constexpr int kCA3AnimDamage = 160.0f;
	//チャージ攻撃の持続
	constexpr float kCA3KeepFrame = 60.0f;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//アニメーション
	const char* kAnim = "Player|CA";//チャージ攻撃
	//チャージ攻撃の段階別アニメーションの速度
	constexpr float kCA3AnimSpeed = 3.0f;
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//左足の半径
	constexpr float kLeftLegRadius = 20.0f;
	//移動速度
	constexpr float kAttackMoveSpeed = 10.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//加算ゲージ量
	constexpr int kAddUltGage = 3;
}

PlayerStateCA3::PlayerStateCA3(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager) :
	PlayerStateBase(player)
{
	m_player->GetCollidable()->SetState(State::None);
	//チャージ攻撃3
	auto model = m_player->GetModel();
	model->SetAnim(kAnim, true, kCA3AnimSpeed);
	model->SetFixedLoopFrame(kCA3KeepFrame);//指定ループ
	//攻撃判定の準備
	CreateAttack();
	//攻撃マネージャー
	auto attackManager = actorManager->GetAttackManager();
	attackManager->Entry(m_attackC);
	//加算ゲージの予約
	m_player->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateCA3::~PlayerStateCA3()
{
	//加算ゲージの予約
	m_player->GetUltGage()->SetPendingUltGage(0);
}
void PlayerStateCA3::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA3::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
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
	auto model = m_player->GetModel();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//削除
		DeleteAttack(attackManager);
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//アニメーションが一周するたびに攻撃判定のリセット
	if (model->IsFinishAnim())
	{
		//初期化
		m_attackC->Init();
	}
	//入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		m_player->GetCollidable()->GetRb()->SetMoveVec(GetForwardVec(input, camera) * kAttackMoveSpeed);
	}
	else
	{
		//少しずつ減速する
		SpeedDown();
	}
	//攻撃の位置更新
	UpdateAttack();
}

void PlayerStateCA3::CreateAttack()
{
	auto model = m_player->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	m_leftLeg = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(toe, kLeftLegRadius),
		std::make_shared<Rigidbody>(root));

	//武器の座標と当たり判定を攻撃に紐図ける
	m_attackC = std::make_shared<MeleeAttack>(m_leftLeg, kCA3AnimDamage, kCA3KeepFrame, kKnockBackPower, m_player);
}

void PlayerStateCA3::UpdateAttack()
{
	//左足の状態を更新したら攻撃も更新される
	auto model = m_player->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	//座標更新
	m_leftLeg->GetRb()->SetPos(root);
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->SetEndPos(toe);
}

void PlayerStateCA3::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateCA3::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃判定を消す
	m_attackC->Delete();
	attackManager->Exit(m_attackC);
}
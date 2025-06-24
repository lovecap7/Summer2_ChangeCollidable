#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/SphereCollider.h"
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
	//Ultのダメージと持続フレーム
	constexpr int kUltDamege = 30.0f;
	constexpr int kUltKeepFrame = 180;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 2.0f;
	//攻撃判定をリセットする頻度
	constexpr int kUltInitFrame = 10;
	//アニメーション
	const char* kAnim = "Player|Ult2";
	//アニメーションの速度の初期値
	constexpr float kAnimSpeed = 0.7f;
	//アニメーションの速度の変化量
	constexpr float kAddAnimSpeed = 0.2f;
	//アニメーションの最高速度
	constexpr float kMaxAnimSpeed = 2.0f;
	//武器の座標と当たり判定の情報
	constexpr float kAttackDistance = 200.0f;//プレイヤーの前方に攻撃判定を出す際の距離
	constexpr float kAttackRadius = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateUltimate::PlayerStateUltimate(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_animCountFrame(0),
	m_animSpeed(kAnimSpeed)
{
	//必殺技
	m_player->GetCollidable()->SetState(State::None);
	//チャージ攻撃1
	auto model = m_player->GetModel();
	model->SetAnim(kAnim, true, m_animSpeed);
	model->SetFixedLoopFrame(kUltKeepFrame);//指定ループ
	//攻撃判定の準備
	CreateAttack();
	//攻撃マネージャーに登録
	auto attackManager = actorManager->GetAttackManager();
	attackManager->Entry(m_attackUlt);
	//向きの更新
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
	//無敵に
	m_player->GetHurtPoint()->SetIsNoDamege(true);
	//ゲージリセット
	m_player->GetUltGage()->ResetUltGage();
}


PlayerStateUltimate::~PlayerStateUltimate()
{
	//無敵解除
	m_player->GetHurtPoint()->SetIsNoDamege(false);
}

void PlayerStateUltimate::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateUltimate::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	auto model = m_player->GetModel();
	//攻撃マネージャー
	auto attackManager = actorManager->GetAttackManager();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//削除
		DeleteAttack(attackManager);
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//攻撃判定をリセット
	if (m_animCountFrame % kUltInitFrame == 0)
	{
		//初期化
		m_attackUlt->Init();
	}
	//アニメーションが一周するたびに再生速度を上げる
	if (model->IsFinishAnim())
	{
		//アニメーションの速度を速くしていく
		if (m_animSpeed < kMaxAnimSpeed)
		{
			m_animSpeed += kAddAnimSpeed;
			model->SetAnimSpeed(m_animSpeed);
		}
	}
	//少しずつ減速する
	SpeedDown();
	//攻撃の位置更新
	UpdateAttack();
}

void PlayerStateUltimate::CreateAttack()
{
	auto model = m_player->GetModel();
	//プレイヤーの前あたりに出す
	Vector3 attackPos = m_player->GetCollidable()->GetRb()->GetPos() + model->GetDir().Normalize() * kAttackDistance;
	//攻撃の位置を出す
	auto attackColl = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kAttackRadius),
		std::make_shared<Rigidbody>(attackPos));
	//攻撃の座標と当たり判定を攻撃に紐図ける
	m_attackUlt = std::make_shared<MeleeAttack>(attackColl, kUltDamege, kUltKeepFrame, kKnockBackPower,m_player);
	//強さを最高に
	m_attackUlt->SetAttackPower(Battle::AttackWeight::Heaviest);
}

void PlayerStateUltimate::UpdateAttack()
{
	auto model = m_player->GetModel();
	//プレイヤーの前あたりに出す
	Vector3 attackPos = m_player->GetCollidable()->GetRb()->GetPos() + model->GetDir().Normalize() * kAttackDistance;
	m_attackUlt->GetCollidable()->GetRb()->SetPos(attackPos);
}

void PlayerStateUltimate::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateUltimate::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃判定を消す
	m_attackUlt->Delete();
	attackManager->Exit(m_attackUlt);
}

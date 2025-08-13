#include "BossKing.h"
#include "BossKingStateStart.h"
#include <memory>
#include <cassert>
#include "../../../ActorManager.h"
#include "../../../Stage/BossArea.h"
#include "../../Player/Player.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/game.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/AttackPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../GameRule/Score.h"
#include "../../../Character/CharacterStateBase.h"
#include "../../../../UI/UIManager.h"
#include "../../../../UI/EnemyUI/BossHPUI.h"

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 80.0f; //カプセルの半径
	//プレイヤーを発見する距離
	constexpr float kSearchDistance = 10000.0f;
	//プレイヤーを発見する視野角
	constexpr float kSearchAngle = 360.0f * MyMath::DEG_2_RAD;
	//体力
	constexpr int kHp = 9000;
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 30;
	//モデルの高さ調整
	constexpr float kModelHeightAdjust = -70.0f;
}

BossKing::BossKing(int modelHandle, int transHandle, Vector3 pos):
	EnemyBase(Shape::Capsule, EnemyGrade::Boss),
	m_isActive(false),
	m_transHandle(transHandle)
{
	//モデルの初期化
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	m_model->SetRotSpeed(kModelRotateSpeed);
	//衝突判定
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //カプセルの上端
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(kCapsuleRadius);
	cap->SetEndPos(endPos);
	//リジッドボディの初期化
	m_rb->SetPos(pos);
	//体力ステータス
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Heavy);
	//攻撃ステータス
	m_attackPoints = std::make_shared<AttackPoints>();
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(kModelHeightAdjust);
}

BossKing::~BossKing()
{
}

void BossKing::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Enemy, false, false, true);
	//Physicsに登録
	Collidable::Init();
	//サウンド
	InitSound();
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<BossKing>(shared_from_this());
	m_state = std::make_shared<BossKingStateStart>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//敵関連のUIの準備
	m_hpUI = UIManager::GetInstance().CreateBossUI(thisPointer);
	m_hpUI.lock()->SetIsDraw(false);
}

void BossKing::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーから遠いなら処理をしない
	if (IsStopActive(actorManager))return;
#if _DEBUG
	//ボスを死亡させる
	if (Input::GetInstance().IsTrigger("BossDead"))
	{
		m_hitPoints->SetIsNoDamege(false);
		m_hitPoints->Damage(999999);
	}
#endif
	//ボス部屋に入った時行動開始
	m_isActive = actorManager.lock()->GetBossArea().lock()->IsEvent();
	//アクティブ状態じゃないなら
	if (!m_isActive)
	{
		//アニメーションの更新
		m_model->Update();
		//無敵に
		m_hitPoints->SetIsNoDamege(true);
		return;
	}
	else
	{
		//無敵解除
		m_hitPoints->SetIsNoDamege(false);
	}
	//体力の表示をする
	if (!m_hpUI.expired())
	{
		auto hpUI = m_hpUI.lock();
		if (!hpUI->IsDraw())
		{
			hpUI->SetIsDraw(true);
		}
	}
	//攻撃のクールタイムを減らす
	UpdateAttackCoolTime();
	//ターゲットを発見できたかをチェック
	auto target = actorManager.lock()->GetPlayer();
	if (!target.expired())
	{
		TargetSearch(kSearchDistance, kSearchAngle, target.lock()->GetPos());
	}
	//状態に合わせた更新
	m_state->Update(camera, actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//体力クラスのフラグリセット
	m_hitPoints->ResetHitFlags();
}

void BossKing::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void BossKing::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_rb->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
	//探索範囲
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(), kSearchDistance, 4, 0x0000ff, 0x0000ff, false);
	//見てる方向
	auto forward = m_model->GetDir();
	forward = forward * kSearchDistance;
	//視野角
	auto viewDir1 = Quaternion::AngleAxis(kSearchAngle / 2.0f, Vector3::Up()) * forward;
	auto viewDir2 = Quaternion::AngleAxis(-kSearchAngle / 2.0f, Vector3::Up()) * forward;
	//描画
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + forward).ToDxLibVector(), 0xff0000);
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir1).ToDxLibVector(), 0xff0000);
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir2).ToDxLibVector(), 0xff0000);
#endif
	m_model->Draw();
}

void BossKing::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//次の座標へ
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void BossKing::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//体力がなくなっていない場合は無視
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kBossMuscle);
	//死亡エフェクト
	EffekseerManager::GetInstance().CreateEffect("BossDeathEff", m_rb->m_pos);
	//爆発SE
	SoundManager::GetInstance().PlayOnceSE("BossBlast");
}

void BossKing::End()
{
	EndSound();
	m_model->End();
	Collidable::End();
}


bool BossKing::IsStartAnim()
{
	//スタート状態以外はfalse
	if (std::dynamic_pointer_cast<BossKingStateStart>(m_state) == nullptr)
	{
		return false;
	}
	return true;
}

void BossKing::TransformSecond()
{
	//モデルを変更
	m_model->SetModel(m_transHandle);
}

void BossKing::FullRecovery()
{
	//全回復
	m_hitPoints->Revival();
}

void BossKing::InitSound()
{
	//SE
	m_soundHandles["MagicCircle"] = LoadSoundMem(L"Data/Sound/SE/Enemy/BossKing/MagicCircle.mp3");
	m_soundHandles["Thunder"] = LoadSoundMem(L"Data/Sound/SE/Enemy/BossKing/Thunder.mp3");
	m_soundHandles["Charge"] = LoadSoundMem(L"Data/Sound/SE/Enemy/BossKing/Charge.mp3");
	//VC
	m_soundHandles["Attack1"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Attack1.wav");
	m_soundHandles["Attack2"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Attack2.wav");
	m_soundHandles["Attack3"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Attack3.wav");
	m_soundHandles["Attack4"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Attack4.wav");
	m_soundHandles["Attack5"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Attack5.wav");
	m_soundHandles["Change"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Change.wav");
	m_soundHandles["Damage"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Damage.wav");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Dead.wav");
	m_soundHandles["Start"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Start.wav");
	m_soundHandles["Ult"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossKing/Ult.wav");
	//ロードに成功したかチェック
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}
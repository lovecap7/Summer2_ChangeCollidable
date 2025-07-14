#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../Attack/Breath.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�e�̔��ˊp�x
	constexpr float kBulletAngle = 30.0f / 180.0f * DX_PI_F;
	//�e�̐����ʒu��Y���W
	constexpr float kBulletCreatePosY = 150.0f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 100;
}


BossDragonStateBreathAttack::BossDragonStateBreathAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBreathName);
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
}

BossDragonStateBreathAttack::~BossDragonStateBreathAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void BossDragonStateBreathAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateBreathAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStateBreathAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack1 = std::dynamic_pointer_cast<Breath>(actorManager.lock()->CreateAttack(AttackType::Breath, m_owner).lock());
	m_attack2 = std::dynamic_pointer_cast<Breath>(actorManager.lock()->CreateAttack(AttackType::Breath, m_owner).lock());
	m_attack3 = std::dynamic_pointer_cast<Breath>(actorManager.lock()->CreateAttack(AttackType::Breath, m_owner).lock());
	//�e�̐ݒ�
	SetupBreath(m_attack1, 0.0f);
	SetupBreath(m_attack2, kBulletAngle);
	SetupBreath(m_attack3, -kBulletAngle);
	//�G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", std::dynamic_pointer_cast<Actor>(m_attack1.lock()));
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", std::dynamic_pointer_cast<Actor>(m_attack2.lock()));
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", std::dynamic_pointer_cast<Actor>(m_attack3.lock()));
}

void BossDragonStateBreathAttack::SetupBreath(std::weak_ptr<Breath> bullet, float angle)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//�e�̐ݒ�
	auto attack = bullet.lock();
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//�����ʒu
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//�e�̐i�s�����ƃX�s�[�h
	auto vec = coll->GetModel()->GetDir() * data.moveSpeed;
	vec = Quaternion::AngleAxis(angle, Vector3::Up()) * vec;
	attack->SetVec(vec);
}

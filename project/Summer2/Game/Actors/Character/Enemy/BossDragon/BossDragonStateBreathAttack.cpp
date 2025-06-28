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
#include "../../../../../Game/Camera/Camera.h"
#include "../../../Attack/Bullet.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�e�̔��a�̑傫��
	constexpr float kBulletRadius = 40.0f;
	//�e�̃_���[�W
	constexpr int kBulletDamage = 100;
	//�e�̎����t���[��
	constexpr int kBulletKeepFrame = 180;
	//�e�̔����t���[��
	constexpr int kBulletFireFrame = 40;
	//�e�̃X�s�[�h
	constexpr float kBulletSpeed = 7.0f;
	//�e�̔��ˊp�x
	constexpr float kBulletAngle = 30.0f / 180.0f * DX_PI_F;
	//�e�̐����ʒu��Y���W
	constexpr float kBulletCreatePosY = 150.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 12.0f;

	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Headbutt";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.2f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 100;
}


BossDragonStateBreathAttack::BossDragonStateBreathAttack(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
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
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kBulletFireFrame)
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
	m_attack1 = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	m_attack2 = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	m_attack3 = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	//�e�̐ݒ�
	SetupBullet(m_attack1, 0.0f);
	SetupBullet(m_attack2, kBulletAngle);
	SetupBullet(m_attack3, -kBulletAngle);
}

void BossDragonStateBreathAttack::SetupBullet(std::weak_ptr<Bullet> bullet, float angle)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//�e�̐ݒ�
	auto attack = bullet.lock();
	attack->SetRadius(kBulletRadius);
	attack->AttackSetting(kBulletDamage, kBulletKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
	//�����ʒu
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//�e�̐i�s�����ƃX�s�[�h
	auto vec = coll->GetModel()->GetDir() * kBulletSpeed;
	vec = Quaternion::AngleAxis(angle, Vector3::Up()) * vec;
	attack->SetVec(vec);
}

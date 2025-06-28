#include "SmallDragonStateAttack.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
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
	constexpr float kBulletRadius = 20.0f;
	//�e�̃_���[�W
	constexpr int kBulletDamage = 100;
	//�e�̎����t���[��
	constexpr int kBulletKeepFrame = 180;
	//�e�̔����t���[��
	constexpr int kBulletFireFrame = 20;
	//�e�̃X�s�[�h
	constexpr float kBulletSpeed = 3.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 4.0f;
	//�����ʒu�̍���
	constexpr float kBulletCreatePosY = 100.0f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Headbutt";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.3f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 150;//2.5�b���炢�̊��o�ōU��
}

SmallDragonStateAttack::SmallDragonStateAttack(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//����̂ق�������
	coll->LookAtTarget();
}

SmallDragonStateAttack::~SmallDragonStateAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void SmallDragonStateAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
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
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		return;
	}
	
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void SmallDragonStateAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(kBulletRadius);
	attack->AttackSetting(kBulletDamage, kBulletKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
	//�����ʒu
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//�e�̐i�s�����ƃX�s�[�h
	attack->SetVec(coll->GetModel()->GetDir() * kBulletSpeed);
}
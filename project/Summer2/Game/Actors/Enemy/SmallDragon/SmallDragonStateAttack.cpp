#include "SmallDragonStateAttack.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/BulletAttack.h"
#include "../../ActorManager.h"

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

	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Headbutt";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.3f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 150;//2.5�b���炢�̊��o�ōU��
}

SmallDragonStateAttack::SmallDragonStateAttack(std::shared_ptr<SmallDragon> owner) :
	SmallDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//�ʏ�U��
	m_owner->GetCollidable()->SetState(State::None);
	//�U��
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//�v���C���[������
	LookPlayer();
}

SmallDragonStateAttack::~SmallDragonStateAttack()
{
	//�U���̃N�[���^�C��
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}
void SmallDragonStateAttack::Init()
{
	// ���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}
	
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kBulletFireFrame)
	{
		//�U������̏���
		CreateAttack();
		//�U��������
		AppearAttack(m_attack, attackManager);
	}
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
	}

	//����
	SpeedDown();
}

void SmallDragonStateAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//�����ʒu
	Vector3 bulletPos = m_owner->GetCollidable()->GetRb()->GetPos();
	bulletPos.y += 100.0f;
	//�e�̓����蔻��쐬
	auto coll = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kBulletRadius),
		std::make_shared<Rigidbody>(bulletPos));
	//�e�̍��W�Ɠ����蔻����U���ɕR�}����
	m_attack = std::make_shared<BulletAttack>(coll, kBulletDamage, kBulletKeepFrame, kKnockBackPower, m_owner);
	//�e�̐i�s�����ƃX�s�[�h
	m_attack->SetDirAndSpeed(model->GetDir(), kBulletSpeed);
}

void SmallDragonStateAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
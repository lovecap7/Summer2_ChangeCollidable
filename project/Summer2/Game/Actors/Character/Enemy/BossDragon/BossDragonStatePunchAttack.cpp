#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/AreaOfEffectAttack.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�E��̃C���f�b�N�X
	constexpr int kRightHandIndex = 36;
	//�p���`�̓����蔻��̑傫��(�U���̑傫��)
	constexpr float kAttackRadius = 100.0f;
	//�U���̃_���[�W
	constexpr int kAttackDamage = 100;
	//�U���̎����t���[��
	constexpr int kAttackKeepFrame = 3;
	//�U���̔����t���[��
	constexpr int kAttackStartFrame = 26;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 20.0f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Punch";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.4f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 40;
}


BossDragonStatePunchAttack::BossDragonStatePunchAttack(std::weak_ptr<Actor> owner) :
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

BossDragonStatePunchAttack::~BossDragonStatePunchAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
}

void BossDragonStatePunchAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStatePunchAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	if (m_attackCountFrame == kAttackStartFrame)
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
	//�U���̈ʒu�X�V
	if (!m_attack.expired()) UpdateAttackPos();

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStatePunchAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<AreaOfEffectAttack>(actorManager.lock()->CreateAttack(AttackType::AreaOfEffect, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(kAttackRadius);
	attack->AttackSetting(kAttackDamage, kAttackKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
}

void BossDragonStatePunchAttack::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	VECTOR rightHand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//�E��
	//���W���Z�b�g
	m_attack.lock()->SetPos(rightHand);
}
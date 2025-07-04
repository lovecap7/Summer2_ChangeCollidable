#include "BossDragonStateSweepAttack.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
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
#include "../../../Attack/Strike.h"
namespace
{
	//�U���̑S�̃t���[��
	constexpr int kAllAttackFrame = 110;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���r�ƍ���̃C���f�b�N�X
	constexpr int kLeftArmIndex = 31;
	constexpr int kLeftHandIndex = 36;
	//�\�����쒆�̉�]
	constexpr float kStartRotaAngle = 1.0f;
	//�A�j���[�V����
	const char* kStartAnim = "CharacterArmature|No";
	//�A�j���[�V�����̑��x
	constexpr float kStartAnimSpeed = 0.5f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 30;
}

BossDragonStateSweepAttack::BossDragonStateSweepAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kSweepName);
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(kStartAnim, false, kStartAnimSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//��]��
	m_attackRotaAngle = -(360.0f + kStartRotaAngle * m_attackData.startFrame) / static_cast<float>(m_attackData.keepFrame);
}

BossDragonStateSweepAttack::~BossDragonStateSweepAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
}

void BossDragonStateSweepAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateSweepAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
		CreateAttack(actorManager);
	}
	//�U�����I������܂ŉ�]
	auto model = coll->GetModel();
	if (m_attackCountFrame >= m_attackData.startFrame)
	{
		//�U�����蒆��]
		if (!m_attack.expired())
		{
			model->SetRot(VGet(0.0f, m_attackRotaAngle, 0.0f));
		}
	}
	else
	{
		//�ŏ��̗\�����쒆��������]
		model->SetRot(VGet(0.0f, kStartRotaAngle, 0.0f));
	}

	//�U���I��
	if (m_attackCountFrame >= kAllAttackFrame)
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

void BossDragonStateSweepAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Strike>(actorManager.lock()->CreateAttack(AttackType::Strike, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(m_attackData.radius);
	attack->AttackSetting(m_attackData.damege, m_attackData.keepFrame,
		m_attackData.knockBackPower, m_attackData.attackWeight);
}

void BossDragonStateSweepAttack::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//�r�Ǝ�̍��W
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//���r
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//��̎w��
	//���W���Z�b�g
	m_attack.lock()->SetStartPos(leftArm);
	m_attack.lock()->SetEndPos(leftHand);
}
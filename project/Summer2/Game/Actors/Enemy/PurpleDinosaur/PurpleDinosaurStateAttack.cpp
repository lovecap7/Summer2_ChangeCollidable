#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../Attack/AttackManager.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/MeleeAttack.h"
#include "../../ActorManager.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���r�ƍ���̃C���f�b�N�X
	constexpr int kLeftArmIndex = 13;
	constexpr int kLeftHandIndex = 17;
	//���r�̓����蔻��̑傫��(�U���̑傫��)
	constexpr float kLeftArmRadius = 20.0f;
	//�U���̃_���[�W
	constexpr int kAttackDamage = 100;
	//�U���̎����t���[��
	constexpr int kAttackKeepFrame = 2;
	//�U���̔����t���[��
	constexpr int kAttackStartFrame = 40;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 3.0f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Weapon";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.3f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 150;//2.5�b���炢�̊��o�ōU��
	//�ړ��t���[��
	constexpr int kMoveFrame = 5;
	//�ړ���
	constexpr float kMoveSpeed = 10.0f;
}

PurpleDinosaurStateAttack::PurpleDinosaurStateAttack(std::shared_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner),
	m_attackCountFrame(0)
{
	//�ʏ�U��
	m_owner->GetCollidable()->SetState(State::None);
	//�U��
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//�U������̏���
	CreateAttack();
	//���f���̌������v���C���[�Ɍ�����
	LookPlayer();
}

PurpleDinosaurStateAttack::~PurpleDinosaurStateAttack()
{
	//�U���̃N�[���^�C��
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void PurpleDinosaurStateAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//�폜
		DeleteAttack(attackManager);
		//���S���
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//�폜
		DeleteAttack(attackManager);
		//������
		ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
		return;
	}
	//�U���̈ʒu�X�V
	UpdateAttack();
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//�U��������
		AppearAttack(m_attack, attackManager);
	}
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//�폜
		DeleteAttack(attackManager);
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}

	//����
	SpeedDown();
	//�U�����ɑO�i����
	AttackMove();
}

void PurpleDinosaurStateAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//�r�Ǝ�̍��W
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//���r
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//��̎w��
	m_leftArm = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(leftArm.x, leftArm.y, leftArm.z), kLeftArmRadius),
		std::make_shared<Rigidbody>(Vector3(leftHand.x, leftHand.y, leftHand.z)));
	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attack = std::make_shared<MeleeAttack>(m_leftArm, kAttackDamage, kAttackKeepFrame, kKnockBackPower,m_owner);
}

void PurpleDinosaurStateAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//�r�Ǝ�̍��W
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//���r
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//��̎w��
	//���r���`������J�v�Z���̍��W���Z�b�g
	m_leftArm->GetRb()->SetPos(Position3(leftArm.x, leftArm.y, leftArm.z));
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->SetEndPos(Position3(leftHand.x, leftHand.y, leftHand.z));
}

void PurpleDinosaurStateAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PurpleDinosaurStateAttack::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//�U�����������
	m_attack->Delete();
	attackManager->Exit(m_attack);
}

void PurpleDinosaurStateAttack::AttackMove()
{
	//�ړ��t���[�����͑O�ɐi��
	if (m_attackCountFrame <= kMoveFrame)
	{
		//�����Ă�����Ɉړ�
		m_owner->GetCollidable()->GetRb()->SetMoveVec(m_owner->GetModel()->GetDir() * kMoveSpeed);
	}
}
#include "BossDragonStateSweepAttack.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
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
#include "../../../../General/Math/Quaternion.h"
namespace
{
	//�U���̑S�̃t���[��
	constexpr int kAllAttackFrame = 110;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���r�ƍ���̃C���f�b�N�X
	constexpr int kLeftArmIndex = 31;
	constexpr int kLeftHandIndex = 36;
	//���r�̓����蔻��̑傫��(�U���̑傫��)
	constexpr float kLeftArmRadius = 20.0f;
	//�U���̃_���[�W
	constexpr int kAttackDamage = 100;
	//�U���̔����t���[��
	constexpr int kAttackStartFrame = 40;
	//�U���̎����t���[��
	constexpr int kAttackKeepFrame = kAllAttackFrame - (kAttackStartFrame + 30);
	//�\�����쒆�̉�]
	constexpr float kStartRotaAngle = 1.0f;
	//�U���̉�]��
	constexpr float kAttackRotaAngle = -(360.0f + kStartRotaAngle * kAttackStartFrame) / static_cast<float>(kAttackKeepFrame);
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 24.0f;
	//�A�j���[�V����
	const char* kAttackAnim = "CharacterArmature|Punch";
	const char* kStartAnim = "CharacterArmature|No";
	//�A�j���[�V�����̑��x
	constexpr float kAttackAnimSpeed = 0.6f;
	constexpr float kStartAnimSpeed = 0.5f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 30;
}


BossDragonStateSweepAttack::BossDragonStateSweepAttack(std::shared_ptr<BossDragon> owner):
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//�ʏ�U��
	m_owner->GetCollidable()->SetState(State::None);
	//�\������
	m_owner->GetModel()->SetAnim(kStartAnim, false, kStartAnimSpeed);
	//�U������̏���
	CreateAttack();
	//�v���C���[������
	LookPlayer();
}

BossDragonStateSweepAttack::~BossDragonStateSweepAttack()
{

	//�U���̃N�[���^�C��
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void BossDragonStateSweepAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}
void BossDragonStateSweepAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//�폜
		DeleteAttack(attackManager);
		//���S���
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//�폜
		DeleteAttack(attackManager);
		//������
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//�U���̈ʒu�X�V
	UpdateAttack();
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//�\������
		m_owner->GetModel()->SetAnim(kAttackAnim, false, kAttackAnimSpeed);
		//�U��������
		AppearAttack(m_attack, attackManager);
	}

	//�U�����I������܂ŉ�]
	auto model = m_owner->GetModel();
	if (m_attackCountFrame >= kAttackStartFrame)
	{
		if (!m_attack->IsDelete())
		{
			model->SetRot(VGet(0.0f, kAttackRotaAngle, 0.0f));
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
		//�폜
		DeleteAttack(attackManager);
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}
	//����
	SpeedDown();
}

void BossDragonStateSweepAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//�r�Ǝ�̍��W
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//���r
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//��̎w��
	m_leftArm = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(leftArm.x, leftArm.y, leftArm.z), kLeftArmRadius),
		std::make_shared<Rigidbody>(Vector3(leftHand.x, leftHand.y, leftHand.z)));
	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attack = std::make_shared<MeleeAttack>(m_leftArm, kAttackDamage, kAttackKeepFrame, kKnockBackPower, m_owner);
}

void BossDragonStateSweepAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//�r�Ǝ�̍��W
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//���r
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//��̎w��
	//���r���`������J�v�Z���̍��W���Z�b�g
	m_leftArm->GetRb()->SetPos(Position3(leftArm.x, leftArm.y, leftArm.z));
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->SetEndPos(Position3(leftHand.x, leftHand.y, leftHand.z));
}

void BossDragonStateSweepAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
void BossDragonStateSweepAttack::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//�U�����������
	m_attack->Delete();
	attackManager->Exit(m_attack);
}
#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
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
	//�E��̃C���f�b�N�X
	constexpr int kRightHandIndex = 36;
	//�p���`�̓����蔻��̑傫��(�U���̑傫��)
	constexpr float kPunchRadius = 100.0f;
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

BossDragonStatePunchAttack::BossDragonStatePunchAttack(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//�ʏ�U��
	m_owner->GetCollidable()->SetState(State::None);
	//�U��
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//�U������̏���
	CreateAttack();
	//�v���C���[������
	LookPlayer();
}

BossDragonStatePunchAttack::~BossDragonStatePunchAttack()
{
	//�U���̃N�[���^�C��
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void BossDragonStatePunchAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStatePunchAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
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
		//�U��������
		AppearAttack(m_attack, attackManager);
	}
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
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

void BossDragonStatePunchAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//�E��̍��W
	VECTOR rightHandPos = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//�E��
	auto punch = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kPunchRadius),
		std::make_shared<Rigidbody>(rightHandPos));
	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attack = std::make_shared<MeleeAttack>(punch, kAttackDamage, kAttackKeepFrame, kKnockBackPower,m_owner);
}

void BossDragonStatePunchAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//�E��̍��W
	VECTOR rightHandPos = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//�E��
	//�E��̍��W���Z�b�g
	m_attack->GetCollidable()->GetRb()->SetPos(rightHandPos);
}

void BossDragonStatePunchAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
void BossDragonStatePunchAttack::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//�U�����������
	m_attack->Delete();
	attackManager->Exit(m_attack);
}

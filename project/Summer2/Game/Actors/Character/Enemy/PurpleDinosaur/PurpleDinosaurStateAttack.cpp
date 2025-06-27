#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../ActorManager.h"
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

PurpleDinosaurStateAttack::PurpleDinosaurStateAttack(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
}

PurpleDinosaurStateAttack::~PurpleDinosaurStateAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void PurpleDinosaurStateAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackStartFrame)
	{
		
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

#include "PlayerStateAttackN1.h"
#include "PlayerStateAttackN2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../Attack/Slash.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�ʏ�U��1�̃_���[�W�Ǝ����t���[���Ɣ����t���[��
	constexpr int kAN1Damege = 100.0f;
	constexpr int kAN1KeepFrame = 6;
	constexpr int kAN1StartFrame = 12;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 3.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|NA1";
	//��U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN1AnimSpeed = 1.3f;
	//�U���I���O�ɃL�����Z���\�t���[��
	constexpr float kAttackCancelFrame = 20.0f;
	//�U���̔��a
	constexpr float kRightSwordRadius = 20.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�U�����q�b�g�����Ƃ��̉��Z�Q�[�W��
	constexpr int kAddUltGage = 1;
	//�ړ��t���[��
	constexpr int kMoveFrame = 5;
	//�ړ���
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN1::PlayerStateAttackN1(std::weak_ptr<Actor> player):
	PlayerStateAttackNormalBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�ʏ�U��1
	coll->SetCollState(CollisionState::Normal);
	//�U��1
	coll->GetModel()->SetAnim(kAnim, false, kAN1AnimSpeed);
	//���Z�Q�[�W�̗\��
	coll->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateAttackN1::~PlayerStateAttackN1()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage()->SetPendingUltGage(0);
}

void PlayerStateAttackN1::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateAttackN1::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���S�����Ȃ�
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	auto model = coll->GetModel();
	//�U�������t���[��
	if (m_attackCountFrame == kAN1StartFrame)
	{
		CreateAttack(kRightSwordRadius,kAN1Damege,kAN1KeepFrame,kKnockBackPower,Battle::AttackWeight::Middle, actorManager);
	}
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
			return;
		}
		//2�i��
		if (input.IsTrigger("X"))
		{
			ChangeState(std::make_shared<PlayerStateAttackN2>(m_owner));
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�`���[�W
			ChangeState(std::make_shared<PlayerStateCharge>(m_owner));
			return;
		}
	}

	//�U���̈ʒu�X�V
	if (!m_attack.expired())
	{
		UpdateSlashAttackPos(m_attack);
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}


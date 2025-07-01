#include "PlayerStateAttackN3.h"
#include "PlayerStateIdle.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateHit.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
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
	//�ʏ�U��3�̃_���[�W�Ǝ����t���[��
	constexpr int kAN3Damege = 300.0f;
	constexpr int kAN3KeepFrame = 13;
	constexpr int kAN3StartFrame = 15;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 4.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|NA3";//�ʏ�U��3
	//��U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN3AnimSpeed = 1.3f;
	//�U���I���O�ɃL�����Z���\�t���[��
	constexpr float kAttackCancelFrame = 30.0f;
	//���a
	constexpr float kRightSwordRadius = 10.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���Z�Q�[�W��
	constexpr int kAddUltGage = 3;
	//�ړ��t���[��
	constexpr int kMoveFrame = 10;
	//�ړ���
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN3::PlayerStateAttackN3(std::weak_ptr<Actor> player) :
	PlayerStateAttackNormalBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�ʏ�U��2
	coll->SetCollState(CollisionState::Move);
	//�U��2
	coll->GetModel()->SetAnim(kAnim, false, kAN3AnimSpeed);
}

PlayerStateAttackN3::~PlayerStateAttackN3()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
}
void PlayerStateAttackN3::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateAttackN3::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���S�����Ȃ�
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAN3StartFrame)
	{
		CreateAttack(kRightSwordRadius, kAN3Damege, kAN3KeepFrame, kKnockBackPower, Battle::AttackWeight::Middle, actorManager);
	}
	auto model = coll->GetModel();
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�3�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
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
		UpdateAttackPos();
	}
	//�ړ��t���[�����͑O�ɐi��
	if (m_attackCountFrame <= kMoveFrame)
	{
		AttackMove(kMoveSpeed);
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
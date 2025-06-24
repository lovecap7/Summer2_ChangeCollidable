#include "PlayerStateAttackN2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttackN3.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/HurtPoint.h"
#include "../../../General/Collision/SearchTrigger.h"
#include "../ActorManager.h"
namespace
{
	//�ʏ�U��2�̃_���[�W�Ǝ����t���[��
	constexpr int kAN2Damege = 200.0f;
	constexpr int kAN2KeepFrame = 6;
	constexpr int kAN2StartFrame = 15;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 3.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|NA2";
	//��U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN2AnimSpeed = 1.3f;
	//�U���I���O�ɃL�����Z���\�t���[��
	constexpr float kAttackCancelFrame = 20.0f;
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒����Ɣ��a
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���Z�Q�[�W��
	constexpr int kAddUltGage = 2;
	//�ړ��t���[��
	constexpr int kMoveFrame = 5;
	//�ړ���
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN2::PlayerStateAttackN2(std::shared_ptr<Player> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	//�ʏ�U��2
	m_player->GetCollidable()->SetState(State::None);
	//�U��2
	m_player->GetModel()->SetAnim(kAnim, false, kAN2AnimSpeed);
	//�U������̏���
	CreateAttack();
	//���Z�Q�[�W�̗\��
	m_player->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateAttackN2::~PlayerStateAttackN2()
{
	//���Z�Q�[�W�̗\�񃊃Z�b�g
	m_player->GetUltGage()->SetPendingUltGage(0);
}
void PlayerStateAttackN2::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateAttackN2::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//���S
	if (m_player->GetHurtPoint()->IsDead())
	{
		//�폜
		DeleteAttack(attackManager);
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//�U�����󂯂���
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//�폜
		DeleteAttack(attackManager);
		//������
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	//�Q�[�W������Ƃ��g����
	if (input.IsTrigger("RB") && m_player->GetUltGage()->IsMaxUlt())
	{
		//�폜
		DeleteAttack(attackManager);
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player, actorManager));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAN2StartFrame)
	{
		//�U��������
		AppearAttack(m_attackN2, attackManager);
	}
	auto model = m_player->GetModel();
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�폜
		DeleteAttack(attackManager);
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�3�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//�폜
			DeleteAttack(attackManager);
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//3�i��
		if (input.IsTrigger("X"))
		{
			//�폜
			DeleteAttack(attackManager);
			ChangeState(std::make_shared<PlayerStateAttackN3>(m_player));
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�폜
			DeleteAttack(attackManager);
			//�`���[�W
			ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//�U���̈ʒu�X�V
	UpdateAttack();
	//��������������
	SpeedDown();
	//�U�����ɑO�i����
	AttackMove();
}

void PlayerStateAttackN2::CreateAttack()
{
	auto model = m_player->GetModel();
	//����̈ʒu���o��
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kRightSwordRadius),
		std::make_shared<Rigidbody>(Vector3(ringFinger.x, ringFinger.y, ringFinger.z)));

	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attackN2 = std::make_shared<MeleeAttack>(m_rightSword, kAN2Damege, kAN2KeepFrame, kKnockBackPower, m_player);
}

void PlayerStateAttackN2::UpdateAttack()
{
	//����̏�Ԃ��X�V������U�����X�V�����

	auto model = m_player->GetModel();
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	//����̎�������Z�b�g
	m_rightSword->GetRb()->SetPos(Position3(ringFinger.x, ringFinger.y, ringFinger.z));
	//����̌�����Z�b�g
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void PlayerStateAttackN2::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
void PlayerStateAttackN2::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//�U�����������
	m_attackN2->Delete();
	attackManager->Exit(m_attackN2);
}

void PlayerStateAttackN2::AttackMove()
{
	//�ړ��t���[�����͑O�ɐi��
	if (m_attackCountFrame <= kMoveFrame)
	{
		//�^�[�Q�b�g�����G���Ă�Ȃ�(�^�[�Q�b�g�̂ق�������)
		auto trigger = m_player->GetSearchTrigger();
		//����
		Vector3 dir = m_player->GetStickVec().XZ();
		//�^�[�Q�b�g�������ł�����
		if (trigger->IsTargetHit())
		{
			auto pToT = trigger->GetTargetPos() - m_player->GetCollidable()->GetRb()->GetPos();
			dir = pToT;
		}
		//�����̍X�V
		m_player->GetModel()->SetDir(dir.XZ());
		//�����Ă�����Ɉړ�
		m_player->GetCollidable()->GetRb()->SetMoveVec(m_player->GetModel()->GetDir() * kMoveSpeed);
	}
}

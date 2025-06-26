#include "PlayerStateAttackN1.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//�ʏ�U��1�̃_���[�W�Ǝ����t���[���Ɣ����t���[��
	constexpr int kAN1Damege = 100.0f;
	constexpr int kAN1KeepFrame = 4;
	constexpr int kAN1StartFrame = 15;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 3.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|NA1";
	//��U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN1AnimSpeed = 1.3f;
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
	//�U�����q�b�g�����Ƃ��̉��Z�Q�[�W��
	constexpr int kAddUltGage = 1;
	//�ړ��t���[��
	constexpr int kMoveFrame = 5;
	//�ړ���
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN1::PlayerStateAttackN1(std::weak_ptr<Player> player):
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	auto coll = m_player.lock();
	//�ʏ�U��1
	coll->SetCollState(CollisionState::Normal);
	//�U��1
	coll->GetModel()->SetAnim(kAnim, false, kAN1AnimSpeed);
	//���Z�Q�[�W�̗\��
	coll->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateAttackN1::~PlayerStateAttackN1()
{
}

void PlayerStateAttackN1::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateAttackN1::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAN1StartFrame)
	{
		//�U��������
		
	}
	auto coll = m_player.lock();
	auto model = coll->GetModel();
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//�폜
			
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//2�i��
		if (input.IsTrigger("X"))
		{
			//�폜
			
			//ChangeState(std::make_shared<PlayerStateAttackN2>(m_player));
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�폜
			
			//�`���[�W
			//ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
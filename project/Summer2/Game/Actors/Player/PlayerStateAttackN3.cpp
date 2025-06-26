#include "PlayerStateAttackN3.h"
#include "PlayerStateIdle.h"
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
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

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
	constexpr int kAddUltGage = 3;
	//�ړ��t���[��
	constexpr int kMoveFrame = 10;
	//�ړ���
	constexpr float kMoveSpeed = 10.0f;
}

PlayerStateAttackN3::PlayerStateAttackN3(std::weak_ptr<Player> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	auto coll = m_player.lock();
	//�ʏ�U��2
	coll->SetCollState(CollisionState::Normal);
	//�U��2
	coll->GetModel()->SetAnim(kAnim, false, kAN3AnimSpeed);
}

PlayerStateAttackN3::~PlayerStateAttackN3()
{
}
void PlayerStateAttackN3::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateAttackN3::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAN3StartFrame)
	{
	
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
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�3�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�`���[�W
			ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
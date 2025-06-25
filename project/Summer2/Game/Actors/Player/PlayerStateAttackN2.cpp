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
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

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

PlayerStateAttackN2::PlayerStateAttackN2(std::weak_ptr<Player> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	auto coll = m_player.lock();
	//�ʏ�U��2
	coll->SetCollState(CollisionState::Normal);
	//�U��2
	coll->GetModel()->SetAnim(kAnim, false, kAN2AnimSpeed);
}

PlayerStateAttackN2::~PlayerStateAttackN2()
{
}
void PlayerStateAttackN2::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateAttackN2::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAN2StartFrame)
	{
		
	}
	auto model = m_player.lock()->GetModel();
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
		//3�i��
		if (input.IsTrigger("X"))
		{
			ChangeState(std::make_shared<PlayerStateAttackN3>(m_player));
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
	SpeedDown();
}

void PlayerStateAttackN2::SpeedDown()
{
	auto collidable = m_player.lock();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
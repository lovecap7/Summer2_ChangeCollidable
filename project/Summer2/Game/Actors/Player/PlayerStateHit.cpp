#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../General/game.h"
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
#include <DxLib.h>
namespace
{
	//�A�j���[�V����
	const char* kHit1Anim = "Player|Hit1";
	const char* kHit2Anim = "Player|Hit2";
	//�A�j���[�V�������x
	constexpr float kHit1AnimSpeed = 1.1f;
	constexpr float kHit2AnimSpeed = 1.1f;
	//������
	constexpr float kDeceRate = 0.95f;
}


PlayerStateHit::PlayerStateHit(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//�����_���Ƀq�b�g�A�j���[�V������I��
	RandHitAnim();
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//���S
	if (m_player->GetHurtPoint()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//���ꃊ�A�N�V�������ɍU����H�������A�j���[�V���������߂���
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//�����_���Ńq�b�g�A�j���[�V������I��
		RandHitAnim();
		//���v���C
		m_player->GetModel()->ReplayAnim();
	}

	//���f���̃A�j���[�V�������I�������
	if (m_player->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//���񂾂񌸑�
	SpeedDown();
}

void PlayerStateHit::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateHit::RandHitAnim()
{
	//�����_���Ńq�b�g�A�j���[�V������I��
	if (GetRand(1))//0��1�Ń����_��
	{
		m_player->GetModel()->SetAnim(kHit1Anim, false, kHit1AnimSpeed);
	}
	else
	{
		m_player->GetModel()->SetAnim(kHit2Anim, false, kHit2AnimSpeed);
	}
}

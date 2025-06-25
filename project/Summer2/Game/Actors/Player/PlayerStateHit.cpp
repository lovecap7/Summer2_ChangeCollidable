#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
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


PlayerStateHit::PlayerStateHit(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//�����_���Ƀq�b�g�A�j���[�V������I��
	RandHitAnim();
	m_player.lock()->SetCollState(CollisionState::Normal);
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const std::weak_ptr<Camera> camera)
{
	//���f���̃A�j���[�V�������I�������
	if (m_player.lock()->GetModel()->IsFinishAnim())
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
	auto collidable = m_player.lock();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateHit::RandHitAnim()
{
	auto coll = m_player.lock();
	//�����_���Ńq�b�g�A�j���[�V������I��
	if (GetRand(1))//0��1�Ń����_��
	{
		coll->GetModel()->SetAnim(kHit1Anim, false, kHit1AnimSpeed);
	}
	else
	{
		coll->GetModel()->SetAnim(kHit2Anim, false, kHit2AnimSpeed);
	}
}

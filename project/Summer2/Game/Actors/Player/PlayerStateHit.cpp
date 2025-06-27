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
	constexpr float kMoveDeceRate = 0.95f;
}


PlayerStateHit::PlayerStateHit(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//�����_���Ƀq�b�g�A�j���[�V������I��
	RandHitAnim();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���f���̃A�j���[�V�������I�������
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//���񂾂񌸑�
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void PlayerStateHit::RandHitAnim()
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
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

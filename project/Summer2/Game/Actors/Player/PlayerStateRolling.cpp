#include "PlayerStateRolling.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//��𑬓x
	constexpr float kRollingMoveSpeed = 10.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Rolling";
	//������[�V�����̑��x
	constexpr float kAnimSpeed = 1.2f;
	//�d�͂��d�߂ɂ���(�⓹�΍�)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}

PlayerStateRolling::PlayerStateRolling(std::weak_ptr<Player> player) :
	PlayerStateBase(player)
{
	//������
	auto coll = m_player.lock();
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	coll->SetCollState(CollisionState::Normal);
	//�����̍X�V
	Vector2 dir = coll->GetStickVec();
	coll->GetModel()->SetDir(dir);
}

PlayerStateRolling::~PlayerStateRolling()
{
	
}
void PlayerStateRolling::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateRolling::Update(const std::weak_ptr<Camera> camera)
{
	auto collidable = m_player.lock();
	//���f���̃A�j���[�V�������I�������
	if (collidable->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto rb = collidable->GetRb();
	//�d��
	rb->AddVec(kBigGravity);
	//�����Ă�����Ɉړ�
	rb->SetMoveVec(collidable->GetModel()->GetDir() * kRollingMoveSpeed);
}

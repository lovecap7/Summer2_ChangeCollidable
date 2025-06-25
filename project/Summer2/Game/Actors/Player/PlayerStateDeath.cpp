#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//�A�j���[�V����
	const char* kAnim = "Player|Dead";
	//������
	constexpr float kDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::weak_ptr<Player> player) :
	PlayerStateBase(player)
{
	auto coll = m_player.lock();
	//�ҋ@���
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Dead);
}

PlayerStateDeath::~PlayerStateDeath()
{
}

void PlayerStateDeath::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update(const std::weak_ptr<Camera> camera)
{
	//�A�j���[�V�����I����
	if (m_player.lock()->GetModel()->IsFinishAnim())
	{
		m_player.lock()->Delete();//�폜
	}
	//����
	SpeedDown();
}

void PlayerStateDeath::SpeedDown()
{
	auto collidable = m_player.lock();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}


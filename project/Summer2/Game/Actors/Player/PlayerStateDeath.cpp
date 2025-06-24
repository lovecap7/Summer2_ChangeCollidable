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
namespace
{
	//�A�j���[�V����
	const char* kAnim = "Player|Dead";
	//������
	constexpr float kDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::shared_ptr<Player> player) :
	PlayerStateBase(player)
{
	//�ҋ@���
	m_player->GetModel()->SetAnim(kAnim, false);
	m_player->GetCollidable()->SetState(State::Dead);
}

PlayerStateDeath::~PlayerStateDeath()
{
}

void PlayerStateDeath::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�A�j���[�V�����I����
	if (m_player->GetModel()->IsFinishAnim())
	{
		m_player->Delete();//�폜
	}
	//����
	SpeedDown();
}

void PlayerStateDeath::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}


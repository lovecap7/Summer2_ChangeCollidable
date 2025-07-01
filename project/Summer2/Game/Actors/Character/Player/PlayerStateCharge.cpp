#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA1.h"
#include "PlayerStateCA2.h"
#include "PlayerStateCA3.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�`���[�W���x��
	constexpr float kChargeLevel1 = 30.0f;
	constexpr float kChargeLevel2 = 60.0f;
	constexpr float kChargeLevel3 = 100.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Charge";//�`���[�W
	//������
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateCharge::PlayerStateCharge(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_chargeFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//������
	coll->SetCollState(CollisionState::Normal);
	//�`���[�W
	coll->GetModel()->SetAnim(kAnim, true);
}

PlayerStateCharge::~PlayerStateCharge()
{
}
void PlayerStateCharge::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCharge::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���S�����Ȃ�
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//����{�^������������
	if (input.IsTrigger("RB"))
	{
		//���
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	//�����̍X�V
	Vector2 dir = coll->GetStickVec();
	coll->GetModel()->SetDir(dir);
	//���߂Ă鎞
	if (input.IsPress("Y"))
	{
		//�^���U���`���[�W
		++m_chargeFrame;
		//�ő�
		if (m_chargeFrame > kChargeLevel3)
		{
			m_chargeFrame = kChargeLevel3;
		}
	}
	//�{�^���𗣂�
	else
	{
		if (m_chargeFrame <= kChargeLevel1)
		{
			//CA1
			ChangeState(std::make_shared<PlayerStateCA1>(m_owner));
			return;
		}
		else if (m_chargeFrame <= kChargeLevel2)
		{
			//CA2
			ChangeState(std::make_shared<PlayerStateCA2>(m_owner));
			return;
		}
		else if (m_chargeFrame <= kChargeLevel3)
		{
			//CA3
			ChangeState(std::make_shared<PlayerStateCA3>(m_owner));
			return;
		}
	}
}

#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
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
	//���������Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//���S�������{�X���|���ĂȂ��ꍇ
	if (coll->GetHitPoints().lock()->IsDead() && !actorManager.lock()->GetBoss().expired())
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
	}
	//�{�^���𗣂�
	else
	{
		ChangeState(std::make_shared<PlayerStateCA>(m_owner, actorManager, m_chargeFrame));
		return;
	}
}

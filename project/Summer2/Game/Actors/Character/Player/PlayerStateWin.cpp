#include "PlayerStateWin.h"
#include "Player.h"
#include "../../../../General/game.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�A�j���[�V����
	const char* kAnim = "Player|Win";
	//������
	constexpr float kMoveDeceRate = 0.95f;
}

PlayerStateWin::PlayerStateWin(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_countFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�������
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
}

PlayerStateWin::~PlayerStateWin()
{
}

void PlayerStateWin::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateWin::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	++m_countFrame;
	if (m_countFrame <= 1)
	{
		//���f���̌������J����������
		auto dir = camera.lock()->GetPos() - coll->GetPos();
		coll->GetModel()->SetDir(dir.XZ());
	}
}


#include "BossDragonStateChase.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
namespace
{
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kBattleDistance = 120.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 8.0f;
}

BossDragonStateChase::BossDragonStateChase(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::None);
	//�A�j���[�V����
	m_owner->GetModel()->SetAnim(kAnim, true);
}

BossDragonStateChase::~BossDragonStateChase()
{
}

void BossDragonStateChase::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateChase::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S���
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}

	//�v���C���[�𔭌������Ƃ�
	if (m_owner->GetSearchTrigger()->IsTargetHit())
	{
		//�v���C���[������
		LookPlayer();
		//�������`�F�b�N
		float dist = m_owner->GetPlayerVec().Magnitude();
		//�퓬��ԋ����Ȃ�
		if (dist <= kBattleDistance)
		{
			//�p���`��Ԃɂ���
			ChangeState(std::make_shared<BossDragonStatePunchAttack>(m_owner));
			return;
		}
		//�˒��͈͊O�Ȃ̂�
		else
		{
			//�v���C���[���ɋ߂Â�
			Vector3 chaseVec = m_owner->GetPlayerNomVecXZ();
			m_owner->GetCollidable()->GetRb()->SetMoveVec(chaseVec * kChaseSpeed);
		}
	}
	//���������Ƃ�
	else
	{
		//�ҋ@��Ԃɂ���
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}
}

#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../../Game/Camera/Camera.h"
namespace
{
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kBattleDistance = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Run";
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 5.0f;
}

BomberStateChase::BomberStateChase(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::None);
	//�A�j���[�V����
	m_owner->GetModel()->SetAnim(kAnim, true);
}

BomberStateChase::~BomberStateChase()
{
}

void BomberStateChase::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateChase::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S���
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
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
			//�ҋ@��Ԃɂ���
			ChangeState(std::make_shared<BomberStateIdle>(m_owner));
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
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
	}
}

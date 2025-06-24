#include "SmallDragonStateBack.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../Actor.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
namespace
{
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kBackDistance = 300.0f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//�v���C���[���痣��鑬�x
	constexpr float kBackSpeed = 1.0f;
}

SmallDragonStateBack::SmallDragonStateBack(std::shared_ptr<SmallDragon> owner):
	SmallDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::None);
	//�A�j���[�V����
	m_owner->GetModel()->SetAnim(kAnim, true);
}

SmallDragonStateBack::~SmallDragonStateBack()
{
}

void SmallDragonStateBack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateBack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S���
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}

	//�v���C���[�𔭌������Ƃ�
	if (m_owner->GetSearchTrigger()->IsTargetHit())
	{
		//�v���C���[������
		LookPlayer();
		//�������`�F�b�N
		float dist = m_owner->GetPlayerVec().Magnitude();
		//�߂��Ȃ狗���Ȃ�
		if (dist <= kBackDistance)
		{
			//�v���C���[���痣���
			Vector3 chaseVec = m_owner->GetPlayerNomVecXZ();
			m_owner->GetCollidable()->GetRb()->SetMoveVec(chaseVec * -kBackSpeed);
		}
		else
		{
			//�ҋ@��Ԃɂ���
			ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		}
	}
	//����������
	else
	{
		//�ҋ@��Ԃɂ���
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
	}
}

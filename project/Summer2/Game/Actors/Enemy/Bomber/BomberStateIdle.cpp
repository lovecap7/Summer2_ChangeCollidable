#include "BomberStateIdle.h"
#include "BomberStateHit.h"
#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateAttack.h"
#include "Bomber.h"
#include "../EnemyBase.h"
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
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Idle";//�ҋ@
}

BomberStateIdle::BomberStateIdle(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner)
{
	//�ҋ@���
	m_owner->GetModel()->SetAnim(kAnim, true);
	m_owner->GetCollidable()->SetState(State::None);
}

BomberStateIdle::~BomberStateIdle()
{
}

void BomberStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
		return;
	}

	//����
	SpeedDown();
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
			//�U���̃N�[���^�C����0�Ȃ�
			if (m_owner->GetAttackCoolTime() <= 0)
			{
				//�U����Ԃɂ���
				ChangeState(std::make_shared<BomberStateAttack>(m_owner));
			}
		}
		//�˒��͈͊O�Ȃ̂�
		else
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<BomberStateChase>(m_owner));
		}
	}
}

void BomberStateIdle::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

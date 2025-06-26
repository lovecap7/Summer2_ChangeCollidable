#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../ActorManager.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�v���C���[�𔭌����鋗��
	constexpr float kSearchDistance = 500.0f;
	//�v���C���[�ɍU�����鋗��
	constexpr float kAttackDistance = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Idle";//�ҋ@
}

PurpleDinosaurStateIdle::PurpleDinosaurStateIdle(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	//�ҋ@���
	m_owner.lock()->GetModel()->SetAnim(kAnim, true);
	m_owner.lock()->SetCollState(CollisionState::Normal);
}

PurpleDinosaurStateIdle::~PurpleDinosaurStateIdle()
{
}

void PurpleDinosaurStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = m_owner.lock();
	//�v���C���[
	auto player = actorManager.lock()->GetPlayer();
	//�������m�F
	auto dis = coll->GetDistanceToPlayer(player);
	//�v���C���[�������鋗��
	if (dis <= kSearchDistance)
	{
		//�v���C���[������
		coll->LookAtPlayer(player);
		//�U���̋���
		if (dis <= kAttackDistance)
		{
			//�U���̃N�[���^�C����0�Ȃ�
			if (coll->GetAttackCoolTime() <= 0)
			{
				//�U����Ԃɂ���
				ChangeState(std::make_shared<PurpleDinosaurStateAttack>(m_owner));
				return;
			}
		}
		//�U���̋�������Ȃ��Ƃ��͋߂Â�
		else
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<PurpleDinosaurStateChase>(m_owner));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
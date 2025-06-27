#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
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
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Run";
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 5.0f;
}

PurpleDinosaurStateChase::PurpleDinosaurStateChase(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
}

PurpleDinosaurStateChase::~PurpleDinosaurStateChase()
{
}

void PurpleDinosaurStateChase::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateChase::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
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
		//�˒��͈͊O�Ȃ̂�
		else
		{
			//�v���C���[���ɋ߂Â�
			Vector3 chaseVec = coll->GetToPlayerNomVecXZ(player);
			coll->GetRb()->SetMoveVec(chaseVec * kChaseSpeed);
			return;
		}
	}
	//�ҋ@���
	ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
	return;
}

#include "BomberStateAttack.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../ActorManager.h"
#include "../../../Item/Bomb.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���r�ƍ���̃C���f�b�N�X
	constexpr int kLeftHandIndex = 17;
	//���r�̓����蔻��̑傫��(�U���̑傫��)
	constexpr float kLeftArmRadius = 20.0f;
	//�U���̔����t���[��
	constexpr int kAttackStartFrame = 40;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Weapon";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.3f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 150;//2.5�b���炢�̊��o�ōU��
	//���e�̏㏸��
	constexpr float kBombUpVecY = 10.0f; //���e�̏㏸��
	//���e�̈ړ���
	constexpr float kBombMoveVecPower = 3.0f; //���e�̈ړ���
}

BomberStateAttack::BomberStateAttack(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//����̂ق�������
	coll->LookAtTarget();
}

BomberStateAttack::~BomberStateAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void BomberStateAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackStartFrame)
	{
		CreateBomb(actorManager);
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BomberStateAttack::CreateBomb(const std::weak_ptr<ActorManager> actorManager)
{
	auto model = m_owner.lock()->GetModel();
	//�����ʒu
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//��̎w��
	auto item = actorManager.lock()->CreateItem(ItemType::Bomb, leftHand).lock();
	//�ړ���
	Vector3 moveVec = model->GetDir();
	moveVec *= kBombMoveVecPower;
	moveVec.y = kBombUpVecY; //�㏸�ʂ�ǉ�
	std::dynamic_pointer_cast<Bomb>(item)->SetVec(moveVec);
}
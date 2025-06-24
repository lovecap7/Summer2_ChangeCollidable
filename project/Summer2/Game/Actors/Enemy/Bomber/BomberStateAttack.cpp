#include "BomberStateAttack.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../Attack/AttackManager.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/MeleeAttack.h"
#include "../../ActorManager.h"
#include "../../Item/Bomb.h"
#include "../../Item/ItemGenerator.h"
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

BomberStateAttack::BomberStateAttack(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner),
	m_attackCountFrame(0)
{
	//�ʏ�U��
	m_owner->GetCollidable()->SetState(State::None);
	//�U��
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//���f���̌������v���C���[�Ɍ�����
	LookPlayer();
}

BomberStateAttack::~BomberStateAttack()
{
	//�U���̃N�[���^�C��
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void BomberStateAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager)
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
	
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//���e�̍쐬
		CreateBomb(actorManager->GetItemGenerator());
	}
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}

	//����
	SpeedDown();
}

void BomberStateAttack::CreateBomb(const std::shared_ptr<ItemGenerator> itemGenerator)
{
	//�����ʒu
	VECTOR leftHand = MV1GetFramePosition(m_owner->GetModel()->GetModelHandle(), kLeftHandIndex);//��̎w��
	//�ړ���
	Vector3 moveVec = m_owner->GetModel()->GetDir();
	moveVec *= kBombMoveVecPower;
	moveVec.y = kBombUpVecY; //�㏸�ʂ�ǉ�
	itemGenerator->GenerateBomb(leftHand, moveVec);
}

void BomberStateAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

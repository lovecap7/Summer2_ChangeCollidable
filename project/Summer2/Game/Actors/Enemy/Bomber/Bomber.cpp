#include "Bomber.h"
#include "BomberStateBase.h"
#include "BomberStateIdle.h"
#include "../EnemyManager.h"
#include <memory>
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../Attack/MeleeAttack.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 40.0f; //�J�v�Z���̔��a
	//�g���K�[�̔��a
	constexpr float kSearchTriggerRadius = 500.0f;
	//�v���C���[��ǂ������鋗��
	constexpr float kRunDistance = 150.0f;
	//�̗�
	constexpr int kHp = 500;
}

Bomber::Bomber(int modelHandle, Vector3 pos):
	EnemyBase(),
	m_attackCoolTime(0)
{
	//���f���̏�����
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//�Փ˔���
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(pos));
	//�R���C�_�u���̏�����
	m_collidable->Init(State::None, Priority::Middle, GameTag::Enemy);
}
Bomber::~Bomber()
{
}

void Bomber::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
	//�o�^
	actorManager->GetEnemyManager()->Entry(shared_from_this());
}

void Bomber::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
	//�o�^����
	actorManager->GetEnemyManager()->Exit(shared_from_this());
}

void Bomber::Init()
{
	//�R���C�_�[�Ɏ����̃|�C���^����������
	m_collidable->SetOwner(shared_from_this());
	//���G�͈�
	m_searchTrigger = std::make_shared<SearchTrigger>(kSearchTriggerRadius, shared_from_this());
	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = shared_from_this();
	m_state = std::make_shared<BomberStateIdle>(thisPointer);
	//���̏�Ԃ�ҋ@��Ԃ�
	m_state->ChangeState(m_state);
	//���ꔻ��(�Փ˔���Ɠ����ɂ���)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, kHp, thisPointer);
}

void Bomber::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager)
{
	//�U���̃N�[���^�C�������炷
	UpdateAttackCoolTime();
	//��Ԃɍ��킹���X�V
	m_state->Update(input, camera, actorManager);
	//��Ԃ��ς���������`�F�b�N
	if (m_state != m_state->GetNextState())
	{
		//��Ԃ�ω�����
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//���ꔻ��̍X�V
	UpdateHurtPoint();
	//�U�����������Ȃ烂�f����Ԃ�����
	if (m_hurtPoint->IsHit())
	{
		//�ԐF��
		m_model->ModelHit();
	}
}

void Bomber::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Bomber::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//�Ȃ�
}

void Bomber::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
	//���ꔻ��
	DrawCapsule3D(
		m_hurtPoint->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetRadius(),
		32,
		0x0000ff,
		0x0000ff,
		m_hurtPoint->IsNoDamege()//���G�̎��͓h��Ԃ����
	);
	//���G�͈�
	DrawSphere3D(
		m_searchTrigger->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		kSearchTriggerRadius,
		16,
		0xff00ff,
		0xff00ff,
		false
	);
#endif
	m_model->Draw();
}

void Bomber::Complete()
{
	//�R���C�_�[
	m_collidable->GetRb()->SetNextPos();//���̍��W��
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//�v���C���[��T���g���K�[
	m_searchTrigger->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Bomber::UpdateHurtPoint()
{
	//�ړ��ʂ��擾
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//���W�X�V
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}

void Bomber::UpdateAttackCoolTime()
{
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}

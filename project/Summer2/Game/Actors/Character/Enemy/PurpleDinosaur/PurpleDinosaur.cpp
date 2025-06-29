#include "PurpleDinosaur.h"
#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaurStateIdle.h"
#include "../../../ActorManager.h"
#include "../../Player/Player.h"
#include <memory>
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/game.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/AttackPoints.h"

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 40.0f; //�J�v�Z���̔��a
	//�̗�
	constexpr int kHp = 500;
	//�v���C���[�𔭌����鋗��
	constexpr float kSearchDistance = 500.0f;
	//�v���C���[�𔭌����鎋��p
	constexpr float kSearchAngle = 180.0f;
}
PurpleDinosaur::PurpleDinosaur(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule)
{
	//���f���̏�����
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//�Փ˔���
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(kCapsuleRadius);
	cap->SetEndPos(endPos);
	//���W�b�h�{�f�B�̏�����
	m_rb->SetPos(pos);
	//�̗̓X�e�[�^�X
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Light);
	//�U���X�e�[�^�X
	m_attackPoints = std::make_shared<AttackPoints>();
}

PurpleDinosaur::~PurpleDinosaur()
{
}

void PurpleDinosaur::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false,true);
	//Physics�ɓo�^
	Collidable::Init();

	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<PurpleDinosaur>(shared_from_this());
	m_state = std::make_shared<PurpleDinosaurStateIdle>(thisPointer);
	//���̏�Ԃ�ҋ@��Ԃ�
	m_state->ChangeState(m_state);
}

void PurpleDinosaur::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�U���̃N�[���^�C�������炷
	UpdateAttackCoolTime();
	//�^�[�Q�b�g�𔭌��ł��������`�F�b�N
	auto target = actorManager.lock()->GetPlayer();
	if (!target.expired())
	{
		TargetSearch(kSearchDistance, kSearchAngle, target.lock()->GetPos());
	}
	//��Ԃɍ��킹���X�V
	m_state->Update(camera,actorManager);
	//��Ԃ��ς���������`�F�b�N
	if (m_state != m_state->GetNextState())
	{
		//��Ԃ�ω�����
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//�A�j���[�V�����̍X�V
	m_model->Update();	
	//�̗̓N���X�̃t���O���Z�b�g
	m_hitPoints->ResetHitFlags();
}

void PurpleDinosaur::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void PurpleDinosaur::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_rb->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void PurpleDinosaur::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void PurpleDinosaur::Dead(const std::weak_ptr<ActorManager> actorManager)
{
	actorManager.lock()->CreateItem(ItemType::Heart, m_rb->GetPos());
}


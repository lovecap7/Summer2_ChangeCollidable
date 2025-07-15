#include "Bomber.h"
#include "BomberStateBase.h"
#include "BomberStateIdle.h"
#include "../../CharacterStateBase.h"
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
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../GameRule/Score.h"

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 40.0f; //�J�v�Z���̔��a
	//�̗�
	constexpr int kHp = 500;
	//�v���C���[�𔭌����鋗��
	constexpr float kSearchDistance = 900.0f;
	//�v���C���[�𔭌����鎋��p
	constexpr float kSearchAngle = 180.0f * MyMath::DEG_2_RAD;
	//�̗̓o�[�\���ʒu
	constexpr float kHPBarY = 30.0f;
	//���f���̐��񑬓x
	constexpr int kRotaSpeed = 30;
}
Bomber::Bomber(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule, EnemyGrade::Normal)
{
	//���f���̏�����
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	m_model->SetRotSpeed(kRotaSpeed);
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

Bomber::~Bomber()
{
}

void Bomber::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false, true);
	//Physics�ɓo�^
	Collidable::Init();

	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<Bomber>(shared_from_this());
	m_state = std::make_shared<BomberStateIdle>(thisPointer);
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
}

void Bomber::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	m_state->Update(camera, actorManager);
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
	//���̈ʒu
	m_hpUIPos = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->m_endPos;
	m_hpUIPos.y += kHPBarY;
}

void Bomber::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Bomber::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
	//�T���͈�
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(), kSearchDistance, 4, 0x0000ff, 0x0000ff, false);
	//���Ă����
	auto forward = m_model->GetDir();
	forward = forward * kSearchDistance;
	//����p
	auto viewDir1 = Quaternion::AngleAxis(kSearchAngle / 2.0f, Vector3::Up()) * forward;
	auto viewDir2 = Quaternion::AngleAxis(-kSearchAngle / 2.0f, Vector3::Up()) * forward;
	//�`��
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + forward).ToDxLibVector(), 0xff0000);
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir1).ToDxLibVector(), 0xff0000);
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir2).ToDxLibVector(), 0xff0000);
#endif
	m_model->Draw();
}

void Bomber::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void Bomber::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//�̗͂��Ȃ��Ȃ��Ă��Ȃ��ꍇ�͖���
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kBomber);
	//�A�C�e���������_���ŗ��Ƃ�
	auto actorM = actorManager.lock();
	if (GetRand(1))
	{
		//�K�E�Q�[�W�A�b�v
		actorM->CreateItem(ItemType::UltGageUp, m_rb->GetPos());
	}
	else
	{
		//�h��o�t
		actorM->CreateItem(ItemType::DefenseUp, m_rb->GetPos());
	}
	//���S�G�t�F�N�g
	EffekseerManager::GetInstance().CreateEffect("DeathEff", m_rb->m_pos);
}

void Bomber::End()
{
	Collidable::End();
	m_model->End();
}


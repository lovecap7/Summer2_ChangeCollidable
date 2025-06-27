#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "../../Attack/AttackBase.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/PolygonCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "UltGage.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 20.0f; //�J�v�Z���̔��a
	//�̗�
	constexpr int kHp = 1000; 
	//�K�E�Z�Q�[�W�̍ő�l
	constexpr int kMaxUltGage = 100;
	//�g���K�[�̔��a
	constexpr float kSearchTriggerRadius = 200.0f;
	//����p
	constexpr float kSearchAngle = 20.0f * MyMath::DEG_2_RAD;
}

Player::Player(int modelHandle, Position3 firstPos) :
	CharacterBase(Shape::Capsule),
	m_stickVec(0.0f,0.0f)
{
	//���W
	m_rb->m_pos = firstPos;
	//�J�v�Z���̏I�_�̐ݒ�
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(endPos);
	cap->SetRadius(kCapsuleRadius);
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�K�E�Z�Q�[�W
	m_ultGage = std::make_shared<UltGage>(kMaxUltGage);
	//�̗�
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Middle);
}

Player::~Player()
{
}

void Player::Init()
{
	//�R���X�g���N�^�őS����������������������
	//shared_from_this()���g���ꍇ�R���X�g���N�^���_�ł�
	//���������m�肵�Ă��Ȃ��̂Ŏg�����Ƃ��ł��Ȃ�
	//�΍�Ƃ���Init���g��

	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false);
	//Physics�ɓo�^
	Collidable::Init();

	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//���̏�Ԃ�ҋ@��Ԃ�
	m_state->ChangeState(m_state);
}

void Player::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	//�X�e�B�b�N�̌���������
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);

#if _DEBUG

#endif
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
void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
	//�U�����󂯂��Ƃ��̏���
	if (other->GetGameTag() == GameTag::Attack)
	{
		OnHitFromAttack(other);
		return;
	}
}

void Player::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false//�n�ʂɂ���Ɠh��Ԃ����
	);

#endif
	m_model->Draw();
}

void Player::Complete()
{
	m_rb->m_pos += m_rb->m_vec;//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateWin.h"
#include "../../Attack/AttackBase.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/AttackPoints.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/PolygonCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../ActorManager.h"
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
	//���G����
	constexpr float kSearchDistance = 200.0f;
	//����p
	constexpr float kSearchAngle = 30.0f;
	//�_�b�V��������ԉ���
	constexpr int kCancelRunFrame = 5;
}

Player::Player(int modelHandle, Position3 firstPos) :
	CharacterBase(Shape::Capsule),
	m_stickVec(0.0f,0.0f),
	m_isRunKeep(false),
	m_cancelRunFrame(0),
	m_initPos{}
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
	//�̗̓X�e�[�^�X
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Light);
	//�U���X�e�[�^�X
	m_attackPoints = std::make_shared<AttackPoints>();
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
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false,true);
	//Physics�ɓo�^
	Collidable::Init();

	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
	//���������W
	m_initPos = m_rb->m_pos;
}

void Player::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���������ۂ̏���
	if (m_rb->m_pos.y < -500.0f)m_rb->m_pos = m_initPos;

	auto& input = Input::GetInstance();
	//�X�e�B�b�N�̌���������
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);

	//�^�[�Q�b�g�𔭌��ł��������`�F�b�N
	auto target = actorManager.lock()->GetNearestEnemy();
	if (!target.expired())
	{
		TargetSearch(kSearchDistance, kSearchAngle, target.lock()->GetPos());
	}
	//������p�����邩
	CheckRunKeep();
	//�K�E�Q�[�W���ő�̎��G�t�F�N�g������
	CheckUltMax();

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
	//�̗̓X�e�[�^�X�̍X�V
	m_hitPoints->Update();
	//�U���X�e�[�^�X�̍X�V
	m_attackPoints->Update();

#if _DEBUG
	if (input.IsTrigger("Max"))
	{
		m_ultGage->AddUltGage(10000);
		m_hitPoints->Heal(10000);
	}
#endif
}

void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
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

void Player::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
}

void Player::End()
{
	Collidable::End();
	m_model->End();
}

bool Player::IsFinishClearAnim()
{
	//������ԈȊO��false
	if (std::dynamic_pointer_cast<PlayerStateWin>(m_state) == nullptr)
	{
		return false;
	}
	//������Ԃ̎��ɃA�j���[�V�������I��������true
	return m_model->IsFinishAnim();
}

void Player::CheckRunKeep()
{
	//�_�b�V����Ԃ���Ȃ��Ƃ�
	if (std::dynamic_pointer_cast<PlayerStateRun>(m_state) == nullptr)
	{
		++m_cancelRunFrame;
		//����
		if (m_cancelRunFrame > kCancelRunFrame)
		{
			m_isRunKeep = false;
			m_cancelRunFrame = 0;
		}
	}
	else
	{
		m_cancelRunFrame = 0;
	}
}

void Player::CheckUltMax()
{
	//�G�t�F�N�g�������ĂȂ��ĕK�E�Q�[�W�ő�Ȃ�G�t�F�N�g������
	if (m_ultMaxEff.expired() && m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff = EffekseerManager::GetInstance().CreateTrackActorEffect("UltGageMaxEff", std::dynamic_pointer_cast<Actor>(shared_from_this()));
	}
	else if (!m_ultMaxEff.expired() && !m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff.lock()->Delete();
	}
}

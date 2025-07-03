#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Character/Player/Player.h"
#include "../Actors/Character/Enemy/EnemyBase.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>
#if _DEBUG
#include "../../General/Input.h"
#endif

namespace
{
	//near��far
	constexpr float kNear = 50.0f;
	constexpr float kFar = 5000.0f;
	//�J�����p�x
	constexpr float kNormalCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	constexpr float kBossCameraAngleX = 40.0f * MyMath::DEG_2_RAD;
	//��ʒ������炠���苗���v���C���[�����ꂽ�ꍇ�Ǐ]����͈�
	constexpr float kChaseWidth = 20.0f;
	//lerp�̊���
	constexpr float kNormalLerpRate = 0.1f;
	constexpr float kBossLerpRate = 0.3f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetNormalCameraPosY = 400.0f;
	constexpr float kOffsetBossCameraPosY = 600.0f;
	constexpr float kOffsetClearCameraPosY = 50.0f;
	constexpr float kOffsetClearCameraPosZ = -300.0f;
	//�N���A���̃^�[�Q�b�g�ւ̒����_�I�t�Z�b�g
	constexpr float kOffsetClearTargetPosY = 100.0f;

	//�N���A���ɉ�]����t���[��
	constexpr int kClearRotaFrame = 300;
}


Camera::Camera(Position3 firstPos):
	m_pos(firstPos),
	m_dir{},
	m_viewPos{},
	m_update(&Camera::NormalUpdate),
	m_rotaFrame(kClearRotaFrame)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kNear, kFar);

	//�J�����̊p�x
	m_dir = Matrix4x4::RotateXMat4x4(kNormalCameraAngleX) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//���Ă�ʒu
	m_viewPos = m_pos + m_dir;

	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//����p
	SetupCamera_Perspective(60.0f / 180.0f * DX_PI_F);
	//�f�B���N�V���i�����C�g
	ChangeLightTypeDir(VGet(0.0f, -0.5f, 0.0f));
}

void Camera::Update(const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(actorManager);
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_dir;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}

void Camera::NormalUpdate(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�{�X�����Ȃ�
	if (actorManager.lock()->IsEntryBossArea())
	{
		//�J�����̊p�x
		m_dir = Matrix4x4::RotateXMat4x4(kBossCameraAngleX) *
			Vector3::Forward();
		if (m_dir.Magnitude() > 0.0f)
		{
			m_dir = m_dir.Normalize();
		}
		//���Ă�ʒu
		m_viewPos = m_pos + m_dir;
		//�{�X�J����
		m_update = &Camera::BossUpdate;
		return;
	}
	//�v���C���[���J�����̓���͈̔͊O�ɏo�悤�Ƃ����ۂɈړ�
	auto playerPos = player.lock()->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 nextPos = m_pos;
	nextPos.y = playerPos.y + kOffsetNormalCameraPosY;//�v���C���[��Y���W��荂���ʒu
	//���������͈͊O�Ȃ�
	if (playerPos.x > m_pos.x + kChaseWidth)//�E
	{
		nextPos.x = playerPos.x;
		nextPos.x -= kChaseWidth;
	}
	else if (playerPos.x < m_pos.x - kChaseWidth)//��
	{
		nextPos.x = playerPos.x;
		nextPos.x += kChaseWidth;
	}
	//���̍��W
	m_pos = Vector3::Lerp(m_pos, nextPos, kNormalLerpRate);
	//���Ă�ʒu
	m_viewPos = m_pos + m_dir;
	//�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

void Camera::BossUpdate(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto boss = actorManager.lock()->GetBoss();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�{�X�����ł�����Q�[���N���A�J������
	if (boss.expired())
	{
		auto player = actorManager.lock()->GetPlayer();
		auto playerPos = player.lock()->GetPos();
		//�ʒu�̍X�V
		Vector3 nextPos = playerPos;
		nextPos.y += kOffsetClearCameraPosY;
		nextPos.z += kOffsetClearCameraPosZ;
		m_pos = nextPos;
		//���Ă�ʒu
		m_viewPos = playerPos;
		m_viewPos.y += kOffsetClearTargetPosY;
		//�N���A�J����
		m_update = &Camera::GameClearUpdate;
		return;
	}
	//�v���C���[�ƃ{�X�̊Ԃ̍��W�ɍ��킹��
	auto playerPos = player.lock()->GetPos();
	auto bossPos = boss.lock()->GetPos();
	//�Ԃ̈ʒu
	auto centerPos = Vector3::Lerp(playerPos, bossPos, kBossLerpRate);
	//�ʒu�̍X�V
	Vector3 nextPos = m_pos;
	nextPos.y = centerPos.y + kOffsetBossCameraPosY;//���S��Y���W��荂���ʒu
	nextPos.x = centerPos.x;
	//���̍��W
	m_pos = Vector3::Lerp(m_pos, nextPos, kNormalLerpRate);
	//���Ă�ʒu
	m_viewPos = m_pos + m_dir;
	//�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

void Camera::GameClearUpdate(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto playerPos = player.lock()->GetPos();
	//���̍��W
	if (m_rotaFrame > 0)
	{
		Quaternion kClearRotaQ = Quaternion::AngleAxis((360.0f / kClearRotaFrame) * MyMath::DEG_2_RAD, Vector3::Up());
		//�ʒu�̍X�V
		Vector3 nextPos = playerPos;
		auto vec = m_pos - playerPos;
		vec = kClearRotaQ * vec;
		nextPos += vec;
		nextPos.y += 0.5f;
		m_pos = nextPos;
		--m_rotaFrame;
	}
	else
	{
		//�ʒu�̍X�V
		Vector3 nextPos = playerPos;
		nextPos.z += -30.0f;
		nextPos.y += 60.0f;
		//���̍��W
		nextPos = Vector3::Lerp(m_pos, nextPos, 0.1f);
		m_pos = nextPos;
		//�����_
		m_viewPos = playerPos;
		m_viewPos.y += 60.0f;
	}
	//�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

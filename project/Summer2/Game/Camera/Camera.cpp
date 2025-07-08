#include "Camera.h"
#include "CameraStateBase.h"
#include "CameraStateNormal.h"
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
	constexpr float kNormalCameraPosZ = -200.0f;
	constexpr float kOffsetBossCameraPosY = 600.0f;
	constexpr float kOffsetClearCameraPosY = 50.0f;
	constexpr float kOffsetClearCameraPosZ = -300.0f;
	//�N���A���̃^�[�Q�b�g�ւ̒����_�I�t�Z�b�g
	constexpr float kOffsetClearTargetPosY = 100.0f;

	//�N���A���ɉ�]����t���[��
	constexpr int kClearRotaFrame = 300;
}


Camera::Camera():
	m_pos{},
	m_dir{},
	m_viewPos{}
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
	ChangeLightTypeDir(m_dir.ToDxLibVector());
	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	m_state = std::make_shared<CameraStateNormal>(shared_from_this());
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
}

void Camera::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//��Ԃɍ��킹���X�V
	m_state->Update(actorManager);
	//��Ԃ��ς���������`�F�b�N
	if (m_state != m_state->GetNextState())
	{
		//��Ԃ�ω�����
		m_state = m_state->GetNextState();
		m_state->Init();
	}
}

Vector3 Camera::GetDir()const
{
	Vector3 dir = m_dir;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}

void Camera::SetDir(Vector3 dir)
{
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	m_dir = dir;
}

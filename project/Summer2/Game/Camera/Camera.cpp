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
}


Camera::Camera():
	m_pos{},
	m_dir{},
	m_viewPos{},
	m_shakePower(ShakePower::None),
	m_shakeFrame(0)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kNear, kFar);
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
	//�J�����V�F�C�N
	UpdateCameraShake();
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

void Camera::UpdateCameraShake()
{
	if (m_shakeFrame > 0)
	{
		auto pos = m_pos;
		auto viewPos = m_viewPos;
		auto shakePower = static_cast<int>(m_shakePower);
		--m_shakeFrame;
		if (m_shakeFrame % 2 == 0)
		{
			shakePower *= -1;
		}
		pos.x += shakePower;
		viewPos.x += shakePower;
		//�U�����߂̎��̂ݏc�h��������
		if (m_shakePower == ShakePower::High)
		{
			pos.y += shakePower;
			viewPos.y += shakePower;
		}
		//�ʒu�X�V
		SetCameraPositionAndTarget_UpVecY(pos.ToDxLibVector(), viewPos.ToDxLibVector());
	}
}

void Camera::SetCameraShake(ShakePower power, int frame)
{
	m_shakePower = power;
	m_shakeFrame = frame;
}

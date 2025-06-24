#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Actor.h"
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
	constexpr float kCameraAngleX = 40.0f * MyMath::DEG_2_RAD;
	//��ʒ������炠���苗���v���C���[�����ꂽ�ꍇ�Ǐ]����͈�
	constexpr float kChaseWidth = 50.0f;
	//Z(���s)�����ɑ΂��ăJ�������Ǐ]����͈͏��
	constexpr float kChaseDepthLimit = 20.0f;
	//lerp�̊���
	constexpr float kLerpRate = 0.05f;
}


Camera::Camera(Position3 firstPos, std::shared_ptr<Actor> player):
	m_pos(firstPos),
	m_player(player)
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kNear, kFar);

	//�J�����̈ʒu�Ɗp�x�̐ݒ�
	auto playerPos = m_player->GetCollidable()->GetRb()->GetPos();
	m_pos.x = playerPos.x;//�v���C���[�Ɖ������ɂ����킹��
	//�J������Z������ۑ�
	m_cameraFirstPosZ = m_pos.z;

	//�J�����̊p�x
	m_dir = Matrix4x4::RotateXMat4x4(kCameraAngleX) *
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

Camera::~Camera()
{
}

void Camera::Update()
{
	//�v���C���[���J�����̓���͈̔͊O�ɏo�悤�Ƃ����ۂɈړ�
	auto playerPos = m_player->GetCollidable()->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 nextPos = m_pos;
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
	//Z�����̈ړ�
	nextPos.z += m_player->GetCollidable()->GetRb()->GetVec().z;
	//�͈͓��Ɏ��߂�
	if (nextPos.z > m_cameraFirstPosZ + kChaseDepthLimit)
	{
		nextPos.z = m_cameraFirstPosZ + kChaseDepthLimit;
	}
	else if(nextPos.z < m_cameraFirstPosZ - kChaseDepthLimit)
	{
		nextPos.z = m_cameraFirstPosZ - kChaseDepthLimit;
	}
	//���̍��W
	m_pos = Vector3::Lerp(m_pos, nextPos, kLerpRate);
	//�����̍X�V
	m_dir = Matrix4x4::RotateXMat4x4(40.0f / 180.0f * DX_PI_F) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//���Ă�ʒu
	m_viewPos = m_pos + m_dir;
	//�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
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
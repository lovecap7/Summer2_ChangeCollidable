#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Character/Player/Player.h"
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
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//��ʒ������炠���苗���v���C���[�����ꂽ�ꍇ�Ǐ]����͈�
	constexpr float kChaseWidth = 20.0f;
	//lerp�̊���
	constexpr float kLerpRate = 0.1f;
}


Camera::Camera(Position3 firstPos):
	m_pos(firstPos),
	m_dir{},
	m_viewPos{}
{
}

Camera::~Camera()
{
}

void Camera::Init(std::weak_ptr<Player> player)
{
	//�v���C���[�̎Q�Ƃ�ۑ�
	m_player = player;
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kNear, kFar);

	//�J�����̈ʒu�Ɗp�x�̐ݒ�
	auto playerPos = m_player.lock()->GetRb()->GetPos();
	m_pos.x = playerPos.x;//�v���C���[�Ɖ������ɂ����킹��

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

void Camera::Update()
{
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (m_player.expired())return;
	//�v���C���[���J�����̓���͈̔͊O�ɏo�悤�Ƃ����ۂɈړ�
	auto playerPos = m_player.lock()->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 nextPos = m_pos;
	nextPos.y = playerPos.y + 400.0f;//�v���C���[��Y���W��荂���ʒu
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
	m_pos = Vector3::Lerp(m_pos, nextPos, kLerpRate);
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
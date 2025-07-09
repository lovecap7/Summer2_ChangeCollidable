#include "Camera.h"
#include "CameraStateNormal.h"
#include "CameraStateAreaLock.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Character/Player/Player.h"
#include "../Actors/Character/Enemy/EnemyBase.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>

namespace
{
	//near��far
	constexpr float kNear = 50.0f;
	constexpr float kFar = 5000.0f;
	//����p
	constexpr float kPerspective = 60.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerp�̊���
	constexpr float kLerpRate = 0.05f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 400.0f;
	constexpr float kCameraPosZ = -200.0f;
}

CameraStateNormal::CameraStateNormal(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	//�J�����̊p�x
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//���Ă�ʒu
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//����p
	SetupCamera_Perspective(kPerspective);
	//�f�B���N�V���i�����C�g
	ChangeLightTypeDir(owner->GetDir().ToDxLibVector());
}

void CameraStateNormal::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void CameraStateNormal::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�C�x���g�G���A�Ȃ�
	if (camera->IsEvent())
	{
		ChangeState(std::make_shared<CameraStateAreaLock>(m_camera));
		return;
	}
	//�v���C���[���J�����̓���͈̔͊O�ɏo�悤�Ƃ����ۂɈړ�
	auto playerPos = player.lock()->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//�v���C���[��Y���W��荂���ʒu
	nextPos.x = playerPos.x;
	//���̍��W
	nextPos = Vector3::Lerp(oldPos, nextPos, kLerpRate);
	//���Ă������
	Vector3 dir = camera->GetDir();
	//���Ă�ʒu
	Vector3 viewPos = nextPos + dir;
	//�ʒu�X�V
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}

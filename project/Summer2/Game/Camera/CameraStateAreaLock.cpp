#include "CameraStateAreaLock.h"
#include "CameraStateClear.h"
#include "CameraStateNormal.h"
#include "Camera.h"
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
	constexpr float kLerpRate = 0.1f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 500.0f;
	constexpr float kCameraPosZ = -300.0f;
	//�ǂ���̋���
	constexpr float kDistanceFromWall = 300.0f;
}

CameraStateAreaLock::CameraStateAreaLock(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	//�J�����̊p�x
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//���Ă�ʒu
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//�J�����̍��W�ƒ����_
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//����p
	DxLib::SetupCamera_Perspective(kPerspective);
}

void CameraStateAreaLock::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void CameraStateAreaLock::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�{�X�����ł�����Q�[���N���A�J������
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	auto camera = m_camera.lock();
	if (!camera->IsEvent())
	{
		//�ʏ�̃J������
		ChangeState(std::make_shared<CameraStateNormal>(m_camera));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�G���A�̎n�_�ƏI�_
	auto startAndEndPosX = camera->StartXAndEndX();
	//�v���C���[�̍��W
	auto playerPos = player.lock()->GetPos();
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//�v���C���[��Y���W��荂���ʒu
	//�G���A�̊O�ɃJ�������߂Â�����~�܂�
	nextPos.x = playerPos.x;
	if (nextPos.x <= startAndEndPosX.x + kDistanceFromWall)
	{
		nextPos.x = startAndEndPosX.x + kDistanceFromWall;
	}
	else if (nextPos.x >= startAndEndPosX.y - kDistanceFromWall)
	{
		nextPos.x = startAndEndPosX.y - kDistanceFromWall;
	}
	//���̍��W
	nextPos = Vector3::Lerp(oldPos, nextPos, kLerpRate);
	//���Ă������
	Vector3 dir = camera->GetDir();
	//���Ă�ʒu
	Vector3 viewPos = nextPos + dir;
	//�ʒu�X�V
	DxLib::SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);


}

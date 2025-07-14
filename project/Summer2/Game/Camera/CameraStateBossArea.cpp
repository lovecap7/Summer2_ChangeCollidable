#include "CameraStateBossArea.h"
#include "CameraStateAreaLock.h"
#include "CameraStateClear.h"
#include "CameraStateNormal.h"
#include "CameraStateBossDeath.h"
#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/game.h"
#include "../../General/HitPoints.h"
#include "../Actors/Character/Player/Player.h"
#include "../Actors/Character/Enemy/EnemyBase.h"
#include "../Actors/ActorManager.h"
#include "../Actors/Stage/EventArea.h"
#include <DxLib.h>

namespace
{
	//near��far
	constexpr float kNear = 50.0f;
	constexpr float kFar = 5000.0f;
	//����p
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerp�̊���
	constexpr float kLerpRateX = 0.05f;
	constexpr float kLerpRateY = 0.05f;
	constexpr float kLerpRateZ = 0.01f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 700.0f;
	constexpr float kOffsetCameraPosZ = -1300.0f;
	//�ǂ���̋���
	constexpr float kDistanceFromWall = 300.0f;
}
CameraStateBossArea::CameraStateBossArea(std::weak_ptr<Camera> camera):
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

void CameraStateBossArea::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void CameraStateBossArea::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto boss = actorManager.lock()->GetBoss();
	//�ʏ�͒ʂ邱�Ƃ͂Ȃ����{�X�����ł�����Q�[���N���A�J������
	if (boss.expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	//�{�X�����S�����ꍇ
	if (boss.lock()->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<CameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	auto camera = m_camera.lock();
	//�C�x���g�����ł�����
	if (camera->GetEventArea().expired())
	{
		//�ʏ�̃J������
		ChangeState(std::make_shared<CameraStateNormal>(m_camera));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�G���A�̎n�_�ƏI�_
	auto area = camera->GetEventArea().lock();
	auto startPos = area->GetEventStartPos();
	auto endPos = area->GetEventEndPos();
	//�v���C���[�̍��W
	auto playerPos = player.lock()->GetPos();
	//�Ԃ̈ʒu
	Vector3 center = (boss.lock()->GetPos() + playerPos) / 2.0f;
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.y = center.y + kOffsetCameraPosY;//�v���C���[��Y���W��荂���ʒu
	//�G���A�̊O�ɃJ�������߂Â�����~�܂�
	nextPos.x = center.x;
	if (nextPos.x <= startPos.x + kDistanceFromWall)
	{
		nextPos.x = startPos.x + kDistanceFromWall;
	}
	else if (nextPos.x >= endPos.x - kDistanceFromWall)
	{
		nextPos.x = endPos.x - kDistanceFromWall;
	}
	nextPos.z = center.z + kOffsetCameraPosZ;
	//���̍��W
	nextPos.x = MathSub::Lerp(oldPos.x, nextPos.x, kLerpRateX);
	nextPos.y = MathSub::Lerp(oldPos.y, nextPos.y, kLerpRateY);
	nextPos.z = MathSub::Lerp(oldPos.z, nextPos.z, kLerpRateZ);
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

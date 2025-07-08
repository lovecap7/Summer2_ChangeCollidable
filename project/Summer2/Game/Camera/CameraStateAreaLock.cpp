#include "CameraStateAreaLock.h"
#include "CameraStateClear.h"
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

CameraStateAreaLock::CameraStateAreaLock(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	//�J�����̊p�x
	owner->SetDir(Matrix4x4::RotateXMat4x4(kBossCameraAngleX) *
		Vector3::Forward());
	//���Ă�ʒu
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
}

void CameraStateAreaLock::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void CameraStateAreaLock::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto boss = actorManager.lock()->GetBoss();
	//�{�X�����ł�����Q�[���N���A�J������
	if (boss.expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�v���C���[�ƃ{�X�̊Ԃ̍��W�ɍ��킹��
	auto playerPos = player.lock()->GetPos();
	auto bossPos = boss.lock()->GetPos();
	//�Ԃ̈ʒu
	auto centerPos = Vector3::Lerp(playerPos, bossPos, kBossLerpRate);
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.y = centerPos.y + kOffsetBossCameraPosY;//���S��Y���W��荂���ʒu
	nextPos.x = centerPos.x;
	//���̍��W
	nextPos = Vector3::Lerp(oldPos, nextPos, kNormalLerpRate);
	//���Ă����
	Vector3 dir = camera->GetDir();
	//���Ă�ʒu
	Vector3 viewPos = nextPos + dir;
	//�ʒu�X�V
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}

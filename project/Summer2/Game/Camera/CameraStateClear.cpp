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
	constexpr float kClearEndLerpRate = 0.05f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	//�N���A���ĉ�]���Ă�Ƃ�
	constexpr float kOffsetClearStartCameraPosY = 50.0f;
	constexpr float kOffsetClearStartCameraPosZ = -500.0f;
	//��]�I����̃A�b�v
	constexpr float kOffsetClearEndCameraPosY = 30.0f;
	constexpr float kOffsetClearEndCameraPosZ = -90.0f;
	//�N���A���̃^�[�Q�b�g�ւ̒����_�I�t�Z�b�g
	constexpr float kOffsetClearStartViewPosY = 100.0f;
	constexpr float kOffsetClearEndViewPosY = 50.0f;

	//�N���A���ɉ�]����t���[��
	constexpr int kClearRotaFrame = 300;
}
CameraStateClear::CameraStateClear(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager):
	CameraStateBase(camera),
	m_rotaFrame(kClearRotaFrame)
{
	auto owner = m_camera.lock();
	auto player = actorManager.lock()->GetPlayer();
	auto playerPos = player.lock()->GetNextPos();
	//�ʒu�̍X�V
	Vector3 nextPos = playerPos;
	nextPos.y += kOffsetClearStartCameraPosY;
	nextPos.z += kOffsetClearStartCameraPosZ;
	owner->SetPos(nextPos);
	//���Ă�ʒu
	auto viewPos = playerPos;
	viewPos.y += kOffsetClearStartViewPosY;
	owner->SetViewPos(viewPos);
}

void CameraStateClear::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void CameraStateClear::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	auto playerPos = player.lock()->GetPos();
	auto viewPos = camera->GetViewPos();
	//���̍��W
	if (m_rotaFrame > 0)
	{
		Quaternion kClearRotaQ = Quaternion::AngleAxis((360.0f / kClearRotaFrame) * MyMath::DEG_2_RAD, Vector3::Up());
		//�ʒu�̍X�V
		Vector3 nextPos = playerPos;
		auto vec = camera->GetPos() - playerPos;
		vec = kClearRotaQ * vec;
		nextPos += vec;
		nextPos.y += 0.5f;
		camera->SetPos(nextPos);
		--m_rotaFrame;
	}
	else
	{
		//�ʒu�̍X�V
		Vector3 nextPos = playerPos;
		nextPos.z += kOffsetClearEndCameraPosZ;
		nextPos.y += kOffsetClearEndCameraPosY;
		//���̍��W
		nextPos = Vector3::Lerp(camera->GetPos(), nextPos, kClearEndLerpRate);
		camera->SetPos(nextPos);
		//�����_
		viewPos = playerPos;
		viewPos.y += kOffsetClearEndViewPosY;
		camera->SetViewPos(viewPos);
	}
	//�ݒ�
	SetCameraPositionAndTarget_UpVecY(camera->GetPos().ToDxLibVector(), viewPos.ToDxLibVector());
}

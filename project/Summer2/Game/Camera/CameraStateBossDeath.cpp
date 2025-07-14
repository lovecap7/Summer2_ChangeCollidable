#include "CameraStateBossDeath.h"
#include "CameraStateClear.h"
#include "CameraStateNormal.h"
#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/HitPoints.h"
#include "../../General/game.h"
#include "../Actors/Character/Player/Player.h"
#include "../Actors/Character/Enemy/EnemyBase.h"
#include "../Actors/ActorManager.h"
#include "../../General/Collision/Physics.h"
#include <DxLib.h>

namespace
{
	//nearとfar
	constexpr float kNear = 50.0f;
	constexpr float kFar = 5000.0f;
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerpの割合
	constexpr float kLerpRateX = 0.05f;
	constexpr float kLerpRateY = 0.05f;
	constexpr float kLerpRateZ = 0.01f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 700.0f;
	constexpr float kOffsetCameraPosZ = -1300.0f;
	//壁からの距離
	constexpr float kDistanceFromWall = 300.0f;
}
CameraStateBossDeath::CameraStateBossDeath(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) :
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	auto boss = actorManager.lock()->GetBoss().lock();
	//位置更新
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), boss->GetPos().ToDxLibVector());
	//撃破ストップ
	actorManager.lock()->DelayUpdate(60);
	owner->SetCameraShake(ShakePower::Highest, 30);
	Physics::GetInstance().DelayUpdate(60);
}

void CameraStateBossDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateBossDeath::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが消滅したらゲームクリアカメラに
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	auto boss = actorManager.lock()->GetBoss().lock();
	auto camera = m_camera.lock();
	//位置更新
	DxLib::SetCameraPositionAndTarget_UpVecY(camera->GetPos().ToDxLibVector(), boss->GetPos().ToDxLibVector());
}

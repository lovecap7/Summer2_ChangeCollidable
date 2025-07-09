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
	//nearとfar
	constexpr float kNear = 50.0f;
	constexpr float kFar = 5000.0f;
	//視野角
	constexpr float kPerspective = 60.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerpの割合
	constexpr float kLerpRate = 0.1f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 500.0f;
	constexpr float kCameraPosZ = -300.0f;
	//壁からの距離
	constexpr float kDistanceFromWall = 300.0f;
}

CameraStateAreaLock::CameraStateAreaLock(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	//カメラの角度
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//見てる位置
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//カメラの座標と注視点
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//視野角
	DxLib::SetupCamera_Perspective(kPerspective);
}

void CameraStateAreaLock::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateAreaLock::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが消滅したらゲームクリアカメラに
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	auto camera = m_camera.lock();
	if (!camera->IsEvent())
	{
		//通常のカメラへ
		ChangeState(std::make_shared<CameraStateNormal>(m_camera));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	//プレイヤーか消滅した場合更新終了
	if (player.expired())return;
	//エリアの始点と終点
	auto startAndEndPosX = camera->StartXAndEndX();
	//プレイヤーの座標
	auto playerPos = player.lock()->GetPos();
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//プレイヤーのY座標より高い位置
	//エリアの外にカメラが近づいたら止まる
	nextPos.x = playerPos.x;
	if (nextPos.x <= startAndEndPosX.x + kDistanceFromWall)
	{
		nextPos.x = startAndEndPosX.x + kDistanceFromWall;
	}
	else if (nextPos.x >= startAndEndPosX.y - kDistanceFromWall)
	{
		nextPos.x = startAndEndPosX.y - kDistanceFromWall;
	}
	//次の座標
	nextPos = Vector3::Lerp(oldPos, nextPos, kLerpRate);
	//見ている向き
	Vector3 dir = camera->GetDir();
	//見てる位置
	Vector3 viewPos = nextPos + dir;
	//位置更新
	DxLib::SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);


}

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
CameraStateBossArea::CameraStateBossArea(std::weak_ptr<Camera> camera):
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

void CameraStateBossArea::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateBossArea::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto boss = actorManager.lock()->GetBoss();
	//通常は通ることはないがボスが消滅したらゲームクリアカメラに
	if (boss.expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	//ボスが死亡した場合
	if (boss.lock()->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<CameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	auto camera = m_camera.lock();
	//イベントが消滅したら
	if (camera->GetEventArea().expired())
	{
		//通常のカメラへ
		ChangeState(std::make_shared<CameraStateNormal>(m_camera));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	//プレイヤーか消滅した場合更新終了
	if (player.expired())return;
	//エリアの始点と終点
	auto area = camera->GetEventArea().lock();
	auto startPos = area->GetEventStartPos();
	auto endPos = area->GetEventEndPos();
	//プレイヤーの座標
	auto playerPos = player.lock()->GetPos();
	//間の位置
	Vector3 center = (boss.lock()->GetPos() + playerPos) / 2.0f;
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.y = center.y + kOffsetCameraPosY;//プレイヤーのY座標より高い位置
	//エリアの外にカメラが近づいたら止まる
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
	//次の座標
	nextPos.x = MathSub::Lerp(oldPos.x, nextPos.x, kLerpRateX);
	nextPos.y = MathSub::Lerp(oldPos.y, nextPos.y, kLerpRateY);
	nextPos.z = MathSub::Lerp(oldPos.z, nextPos.z, kLerpRateZ);
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

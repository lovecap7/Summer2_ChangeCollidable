#include "Camera.h"
#include "CameraStateNormal.h"
#include "CameraStateAreaLock.h"
#include "CameraStateBossArea.h"
#include "CameraStateClear.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Character/Player/Player.h"
#include "../Actors/Character/Enemy/EnemyBase.h"
#include "../Actors/Stage/BossArea.h"
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
	constexpr float kLerpRate = 0.07f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 800.0f;
	constexpr float kCameraPosZ = -900.0f;
}

CameraStateNormal::CameraStateNormal(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	//カメラの角度
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//見てる位置
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//視野角
	SetupCamera_Perspective(kPerspective);
}

void CameraStateNormal::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateNormal::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが消滅したらゲームクリアカメラに
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	//プレイヤーが消滅した場合更新終了
	if (player.expired())return;
	//ボスエリアにプレイヤーが入ったなら
	if (!actorManager.lock()->GetBossArea().expired())
	{
		if (actorManager.lock()->GetBossArea().lock()->IsEvent())
		{
			ChangeState(std::make_shared<CameraStateBossArea>(m_camera));
			return;
		}
	}
	//イベントエリアにプレイヤーが入ったなら
	if (!camera->GetEventArea().expired())
	{
		if (camera->GetEventArea().lock()->IsEvent())
		{
			ChangeState(std::make_shared<CameraStateAreaLock>(m_camera));
			return;
		}
	}
	//プレイヤーがカメラの特定の範囲外に出ようとした際に移動
	auto playerPos = player.lock()->GetRb()->GetPos();
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//プレイヤーのY座標より高い位置
	nextPos.x = playerPos.x;
	//次の座標
	nextPos = Vector3::Lerp(oldPos, nextPos, kLerpRate);
	//見ている向き
	Vector3 dir = camera->GetDir();
	//見てる位置
	Vector3 viewPos = nextPos + dir;
	//位置更新
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}

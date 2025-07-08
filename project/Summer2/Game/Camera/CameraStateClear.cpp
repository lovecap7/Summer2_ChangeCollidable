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
	//nearとfar
	constexpr float kNear = 50.0f;
	constexpr float kFar = 5000.0f;
	//カメラ角度
	constexpr float kNormalCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	constexpr float kBossCameraAngleX = 40.0f * MyMath::DEG_2_RAD;
	//画面中央からある一定距離プレイヤーが離れた場合追従する範囲
	constexpr float kChaseWidth = 20.0f;
	//lerpの割合
	constexpr float kNormalLerpRate = 0.1f;
	constexpr float kBossLerpRate = 0.3f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetNormalCameraPosY = 400.0f;
	constexpr float kNormalCameraPosZ = -200.0f;
	constexpr float kOffsetBossCameraPosY = 600.0f;
	constexpr float kOffsetClearCameraPosY = 50.0f;
	constexpr float kOffsetClearCameraPosZ = -300.0f;
	//クリア時のターゲットへの注視点オフセット
	constexpr float kOffsetClearTargetPosY = 100.0f;

	//クリア時に回転するフレーム
	constexpr int kClearRotaFrame = 300;
}
CameraStateClear::CameraStateClear(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager):
	CameraStateBase(camera),
	m_rotaFrame(kClearRotaFrame)
{
	auto owner = m_camera.lock();
	auto player = actorManager.lock()->GetPlayer();
	auto playerPos = player.lock()->GetPos();
	//位置の更新
	Vector3 nextPos = playerPos;
	nextPos.y += kOffsetClearCameraPosY;
	nextPos.z += kOffsetClearCameraPosZ;
	owner->SetPos(nextPos);
	//見てる位置
	auto viewPos = playerPos;
	viewPos.y += kOffsetClearTargetPosY;
	owner->SetViewPos(viewPos);
}

void CameraStateClear::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateClear::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	auto playerPos = player.lock()->GetPos();
	auto viewPos = camera->GetViewPos();
	//次の座標
	if (m_rotaFrame > 0)
	{
		Quaternion kClearRotaQ = Quaternion::AngleAxis((360.0f / kClearRotaFrame) * MyMath::DEG_2_RAD, Vector3::Up());
		//位置の更新
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
		//位置の更新
		Vector3 nextPos = playerPos;
		nextPos.z += -30.0f;
		nextPos.y += 60.0f;
		//次の座標
		nextPos = Vector3::Lerp(camera->GetPos(), nextPos, 0.1f);
		camera->SetPos(nextPos);
		//注視点
		viewPos = playerPos;
		viewPos.y += 60.0f;
		camera->SetViewPos(viewPos);
	}
	//設定
	SetCameraPositionAndTarget_UpVecY(camera->GetPos().ToDxLibVector(), viewPos.ToDxLibVector());
}

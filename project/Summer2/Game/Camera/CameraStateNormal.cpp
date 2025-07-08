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

CameraStateNormal::CameraStateNormal(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
}

void CameraStateNormal::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateNormal::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	//プレイヤーが消滅した場合更新終了
	if (player.expired())return;
	//ボス部屋なら
	if (actorManager.lock()->IsEntryBossArea())
	{
		ChangeState(std::make_shared<CameraStateAreaLock>(m_camera));
		return;
	}
	//プレイヤーがカメラの特定の範囲外に出ようとした際に移動
	auto playerPos = player.lock()->GetRb()->GetPos();
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kNormalCameraPosZ;
	nextPos.y = playerPos.y + kOffsetNormalCameraPosY;//プレイヤーのY座標より高い位置
	//横方向が範囲外なら
	if (playerPos.x > oldPos.x + kChaseWidth)//右
	{
		nextPos.x = playerPos.x;
		nextPos.x -= kChaseWidth;
	}
	else if (playerPos.x < oldPos.x - kChaseWidth)//左
	{
		nextPos.x = playerPos.x;
		nextPos.x += kChaseWidth;
	}
	//次の座標
	nextPos = Vector3::Lerp(oldPos, nextPos, kNormalLerpRate);
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

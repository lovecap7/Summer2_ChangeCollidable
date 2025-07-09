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
	//lerpの割合
	constexpr float kNormalLerpRate = 0.05f;
	constexpr float kBossLerpRate = 0.3f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetNormalCameraPosY = 400.0f;
	constexpr float kNormalCameraPosZ = -200.0f;
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
	//イベントエリアなら
	if (camera->IsEvent())
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
	nextPos.x = playerPos.x;
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

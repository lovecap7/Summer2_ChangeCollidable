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

CameraStateAreaLock::CameraStateAreaLock(std::weak_ptr<Camera> camera):
	CameraStateBase(camera)
{
	auto owner = m_camera.lock();
	//カメラの角度
	owner->SetDir(Matrix4x4::RotateXMat4x4(kBossCameraAngleX) *
		Vector3::Forward());
	//見てる位置
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
}

void CameraStateAreaLock::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void CameraStateAreaLock::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto boss = actorManager.lock()->GetBoss();
	//ボスが消滅したらゲームクリアカメラに
	if (boss.expired())
	{
		ChangeState(std::make_shared<CameraStateClear>(m_camera, actorManager));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	//プレイヤーか消滅した場合更新終了
	if (player.expired())return;
	//プレイヤーとボスの間の座標に合わせる
	auto playerPos = player.lock()->GetPos();
	auto bossPos = boss.lock()->GetPos();
	//間の位置
	auto centerPos = Vector3::Lerp(playerPos, bossPos, kBossLerpRate);
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.y = centerPos.y + kOffsetBossCameraPosY;//中心のY座標より高い位置
	nextPos.x = centerPos.x;
	//次の座標
	nextPos = Vector3::Lerp(oldPos, nextPos, kNormalLerpRate);
	//見てる向き
	Vector3 dir = camera->GetDir();
	//見てる位置
	Vector3 viewPos = nextPos + dir;
	//位置更新
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}

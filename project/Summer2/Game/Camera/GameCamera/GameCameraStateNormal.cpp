#include "GameCamera.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateZMove.h"
#include "GameCameraStateBossArea.h"
#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/Stage/BossArea.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include <DxLib.h>

namespace
{
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

GameCameraStateNormal::GameCameraStateNormal(std::weak_ptr<GameCamera> camera):
	GameCameraStateBase(camera)
{
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//カメラの角度
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//視野角
	SetupCamera_Perspective(kPerspective);
}

void GameCameraStateNormal::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateNormal::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが死亡した場合
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	//プレイヤーが消滅した場合更新終了
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	auto camera = m_camera.lock();
	//イベントエリアにプレイヤーが入ったなら
	if (!camera->GetEventArea().expired())
	{
		if (camera->GetEventArea().lock()->IsEvent())
		{
			//入ったエリアに応じて状態変化
			auto camera = m_camera.lock();
			switch (camera->GetEventArea().lock()->GetAreaTag())
			{
			case AreaTag::AllKill:
				//全滅エリア
				ChangeState(std::make_shared<GameCameraStateAreaLock>(m_camera));
				return;
				//Z軸移動エリア
			case AreaTag::ZMove:
				ChangeState(std::make_shared<GameCameraStateZMove>(m_camera));
				return;
			case AreaTag::Boss:
				//ボスエリア
				ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
				return;
			}
		}
	}
	//プレイヤーがカメラの特定の範囲外に出ようとした際に移動
	auto playerPos = player->GetRb()->GetPos();
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
	Vector3 viewPos = camera->GetViewPos();
	viewPos = nextPos + dir;
	//位置更新
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}
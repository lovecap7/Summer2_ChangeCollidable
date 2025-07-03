#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Character/Player/Player.h"
#include "../Actors/Character/Enemy/EnemyBase.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>
#if _DEBUG
#include "../../General/Input.h"
#endif

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
	constexpr float kOffsetBossCameraPosY = 600.0f;
	constexpr float kOffsetClearCameraPosY = 50.0f;
	constexpr float kOffsetClearCameraPosZ = -300.0f;
	//クリア時のターゲットへの注視点オフセット
	constexpr float kOffsetClearTargetPosY = 100.0f;

	//クリア時に回転するフレーム
	constexpr int kClearRotaFrame = 300;
}


Camera::Camera(Position3 firstPos):
	m_pos(firstPos),
	m_dir{},
	m_viewPos{},
	m_update(&Camera::NormalUpdate),
	m_rotaFrame(kClearRotaFrame)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);

	//カメラの角度
	m_dir = Matrix4x4::RotateXMat4x4(kNormalCameraAngleX) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//見てる位置
	m_viewPos = m_pos + m_dir;

	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//視野角
	SetupCamera_Perspective(60.0f / 180.0f * DX_PI_F);
	//ディレクショナルライト
	ChangeLightTypeDir(VGet(0.0f, -0.5f, 0.0f));
}

void Camera::Update(const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(actorManager);
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_dir;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}

void Camera::NormalUpdate(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	//プレイヤーが消滅した場合更新終了
	if (player.expired())return;
	//ボス部屋なら
	if (actorManager.lock()->IsEntryBossArea())
	{
		//カメラの角度
		m_dir = Matrix4x4::RotateXMat4x4(kBossCameraAngleX) *
			Vector3::Forward();
		if (m_dir.Magnitude() > 0.0f)
		{
			m_dir = m_dir.Normalize();
		}
		//見てる位置
		m_viewPos = m_pos + m_dir;
		//ボスカメラ
		m_update = &Camera::BossUpdate;
		return;
	}
	//プレイヤーがカメラの特定の範囲外に出ようとした際に移動
	auto playerPos = player.lock()->GetRb()->GetPos();
	//位置の更新
	Vector3 nextPos = m_pos;
	nextPos.y = playerPos.y + kOffsetNormalCameraPosY;//プレイヤーのY座標より高い位置
	//横方向が範囲外なら
	if (playerPos.x > m_pos.x + kChaseWidth)//右
	{
		nextPos.x = playerPos.x;
		nextPos.x -= kChaseWidth;
	}
	else if (playerPos.x < m_pos.x - kChaseWidth)//左
	{
		nextPos.x = playerPos.x;
		nextPos.x += kChaseWidth;
	}
	//次の座標
	m_pos = Vector3::Lerp(m_pos, nextPos, kNormalLerpRate);
	//見てる位置
	m_viewPos = m_pos + m_dir;
	//設定
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

void Camera::BossUpdate(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto boss = actorManager.lock()->GetBoss();
	//プレイヤーか消滅した場合更新終了
	if (player.expired())return;
	//ボスが消滅したらゲームクリアカメラに
	if (boss.expired())
	{
		auto player = actorManager.lock()->GetPlayer();
		auto playerPos = player.lock()->GetPos();
		//位置の更新
		Vector3 nextPos = playerPos;
		nextPos.y += kOffsetClearCameraPosY;
		nextPos.z += kOffsetClearCameraPosZ;
		m_pos = nextPos;
		//見てる位置
		m_viewPos = playerPos;
		m_viewPos.y += kOffsetClearTargetPosY;
		//クリアカメラ
		m_update = &Camera::GameClearUpdate;
		return;
	}
	//プレイヤーとボスの間の座標に合わせる
	auto playerPos = player.lock()->GetPos();
	auto bossPos = boss.lock()->GetPos();
	//間の位置
	auto centerPos = Vector3::Lerp(playerPos, bossPos, kBossLerpRate);
	//位置の更新
	Vector3 nextPos = m_pos;
	nextPos.y = centerPos.y + kOffsetBossCameraPosY;//中心のY座標より高い位置
	nextPos.x = centerPos.x;
	//次の座標
	m_pos = Vector3::Lerp(m_pos, nextPos, kNormalLerpRate);
	//見てる位置
	m_viewPos = m_pos + m_dir;
	//設定
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

void Camera::GameClearUpdate(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto playerPos = player.lock()->GetPos();
	//次の座標
	if (m_rotaFrame > 0)
	{
		Quaternion kClearRotaQ = Quaternion::AngleAxis((360.0f / kClearRotaFrame) * MyMath::DEG_2_RAD, Vector3::Up());
		//位置の更新
		Vector3 nextPos = playerPos;
		auto vec = m_pos - playerPos;
		vec = kClearRotaQ * vec;
		nextPos += vec;
		nextPos.y += 0.5f;
		m_pos = nextPos;
		--m_rotaFrame;
	}
	else
	{
		//位置の更新
		Vector3 nextPos = playerPos;
		nextPos.z += -30.0f;
		nextPos.y += 60.0f;
		//次の座標
		nextPos = Vector3::Lerp(m_pos, nextPos, 0.1f);
		m_pos = nextPos;
		//注視点
		m_viewPos = playerPos;
		m_viewPos.y += 60.0f;
	}
	//設定
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

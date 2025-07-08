#include "Camera.h"
#include "CameraStateBase.h"
#include "CameraStateNormal.h"
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
	constexpr float kNormalCameraPosZ = -200.0f;
	constexpr float kOffsetBossCameraPosY = 600.0f;
	constexpr float kOffsetClearCameraPosY = 50.0f;
	constexpr float kOffsetClearCameraPosZ = -300.0f;
	//クリア時のターゲットへの注視点オフセット
	constexpr float kOffsetClearTargetPosY = 100.0f;

	//クリア時に回転するフレーム
	constexpr int kClearRotaFrame = 300;
}


Camera::Camera():
	m_pos{},
	m_dir{},
	m_viewPos{}
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
	ChangeLightTypeDir(m_dir.ToDxLibVector());
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	m_state = std::make_shared<CameraStateNormal>(shared_from_this());
	//状態を変化する
	m_state->ChangeState(m_state);
}

void Camera::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//状態に合わせた更新
	m_state->Update(actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
}

Vector3 Camera::GetDir()const
{
	Vector3 dir = m_dir;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}

void Camera::SetDir(Vector3 dir)
{
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	m_dir = dir;
}

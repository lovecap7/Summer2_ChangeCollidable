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
}


Camera::Camera():
	m_pos{},
	m_dir{},
	m_viewPos{},
	m_shakePower(ShakePower::None),
	m_shakeFrame(0),
	m_maxShakeFrame(0)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
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
	//カメラシェイク
	UpdateCameraShake();
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

void Camera::UpdateCameraShake()
{
	//カメラを揺らす
	if (m_shakeFrame > 0)
	{
		auto pos = m_pos;
		auto viewPos = m_viewPos;
		float rate = static_cast<float>(m_shakeFrame) / static_cast<float>(m_maxShakeFrame);
		auto shakePower = static_cast<int>(m_shakePower) * rate;
		--m_shakeFrame;
		//左右に揺らす
		if (m_shakeFrame % 2 == 0)
		{
			shakePower *= -1;
		}
		pos.x += shakePower;
		viewPos.x += shakePower;
		//位置更新
		SetCameraPositionAndTarget_UpVecY(pos.ToDxLibVector(), viewPos.ToDxLibVector());
	}
}

void Camera::SetCameraShake(ShakePower power, int frame)
{
	m_shakePower = power;
	m_shakeFrame = frame;
	m_maxShakeFrame = m_shakeFrame;
}

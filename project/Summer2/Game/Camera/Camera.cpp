#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Actor.h"
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
	constexpr float kCameraAngleX = 40.0f * MyMath::DEG_2_RAD;
	//画面中央からある一定距離プレイヤーが離れた場合追従する範囲
	constexpr float kChaseWidth = 50.0f;
	//Z(奥行)方向に対してカメラが追従する範囲上限
	constexpr float kChaseDepthLimit = 20.0f;
	//lerpの割合
	constexpr float kLerpRate = 0.05f;
}


Camera::Camera(Position3 firstPos, std::shared_ptr<Actor> player):
	m_pos(firstPos),
	m_player(player)
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);

	//カメラの位置と角度の設定
	auto playerPos = m_player->GetCollidable()->GetRb()->GetPos();
	m_pos.x = playerPos.x;//プレイヤーと横方向にを合わせる
	//カメラのZ方向を保存
	m_cameraFirstPosZ = m_pos.z;

	//カメラの角度
	m_dir = Matrix4x4::RotateXMat4x4(kCameraAngleX) *
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

Camera::~Camera()
{
}

void Camera::Update()
{
	//プレイヤーがカメラの特定の範囲外に出ようとした際に移動
	auto playerPos = m_player->GetCollidable()->GetRb()->GetPos();
	//位置の更新
	Vector3 nextPos = m_pos;
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
	//Z方向の移動
	nextPos.z += m_player->GetCollidable()->GetRb()->GetVec().z;
	//範囲内に収める
	if (nextPos.z > m_cameraFirstPosZ + kChaseDepthLimit)
	{
		nextPos.z = m_cameraFirstPosZ + kChaseDepthLimit;
	}
	else if(nextPos.z < m_cameraFirstPosZ - kChaseDepthLimit)
	{
		nextPos.z = m_cameraFirstPosZ - kChaseDepthLimit;
	}
	//次の座標
	m_pos = Vector3::Lerp(m_pos, nextPos, kLerpRate);
	//向きの更新
	m_dir = Matrix4x4::RotateXMat4x4(40.0f / 180.0f * DX_PI_F) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//見てる位置
	m_viewPos = m_pos + m_dir;
	//設定
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
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
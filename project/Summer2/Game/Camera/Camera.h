#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Actor;
class Rigidbody;
class Camera
{
public:
	Camera(Position3 firstPos, std::shared_ptr<Actor> player);
	~Camera();
	void Update();
	//カメラの座標
	Position3 GetPos() { return m_pos; }
	//カメラの向き
	Vector3 GetDir();
private:
	//自分の座標
	Position3 m_pos;
	//カメラの向き
	Vector3 m_dir;
	//カメラが見てる位置
	Vector3 m_viewPos;
	//プレイヤー
	std::shared_ptr<Actor> m_player;
	//カメラの最初のＺ座標(カメラのZ方向の移動上限に使う)
	float m_cameraFirstPosZ;
};


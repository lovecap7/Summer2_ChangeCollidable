#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Player;
class Rigidbody;
class Camera
{
public:
	Camera(Position3 firstPos);
	~Camera();
	//初期化処理
	void Init(std::weak_ptr<Player> player);
	//更新処理
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
	std::weak_ptr<Player> m_player;
	//カメラの最初のＺ座標(カメラのZ方向の移動上限に使う)
	float m_cameraFirstPosZ;
};


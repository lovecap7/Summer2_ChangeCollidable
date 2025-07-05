#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class ActorManager;
class Rigidbody;
class Camera
{
public:
	Camera();
	~Camera();
	//初期化処理
	void Init();
	//更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager);
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
	//クリア時の回転フレーム
	int m_rotaFrame;
private:
	//状態遷移
	using UpdateFunc_t = void(Camera::*)(const std::weak_ptr<ActorManager> actorManager);
	UpdateFunc_t m_update;
	//通常のカメラ
	void NormalUpdate(const std::weak_ptr<ActorManager> actorManager);
	//ボス戦のカメラ
	void BossUpdate(const std::weak_ptr<ActorManager> actorManager);
	//クリア時のカメラ
	void GameClearUpdate(const std::weak_ptr<ActorManager> actorManager);
};


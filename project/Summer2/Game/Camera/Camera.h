#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class ActorManager;
class Rigidbody;
class CameraStateBase;
class Camera:
	public std::enable_shared_from_this<Camera>
{
public:
	Camera();
	~Camera();
	//初期化処理
	void Init();
	//更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager);
	//カメラの座標
	Position3 GetPos() const { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	//カメラの向き
	Vector3 GetDir()const;
	void SetDir(Vector3 dir);
	//カメラの見ている位置
	Vector3 GetViewPos()const { return m_viewPos; };
	void SetViewPos(Vector3 viewPos) { m_viewPos = viewPos; };

	//イベントエリア内での処理に必要
	//イベント開始
	void EventStart(float startPosX, float endPosX);
	//イベント終了
	void EventEnd();
	//イベント中
	bool IsEvent()const { return m_isEvent; };
	//スタートとエンドのX座標
	Vector2 StartXAndEndX()const { return m_startXAndEndX; };
private:
	//自分の座標
	Position3 m_pos;
	//カメラの向き
	Vector3 m_dir;
	//カメラが見てる位置
	Vector3 m_viewPos;
	//イベント中フラグ
	bool m_isEvent;
	//エリアのX座標
	Vector2 m_startXAndEndX;
private:
	//状態遷移
	std::shared_ptr<CameraStateBase> m_state;
};


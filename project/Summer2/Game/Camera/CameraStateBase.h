#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Camera;
class ActorManager;
class CameraStateBase abstract
{
public:
	CameraStateBase(std::weak_ptr<Camera> camera);
	virtual ~CameraStateBase() {};
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const std::weak_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<CameraStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<CameraStateBase> nextState);
private:
	//カメラの状態
	std::shared_ptr<CameraStateBase> m_nextState;
protected:
	//カメラの持ち主
	std::weak_ptr<Camera> m_camera;
};


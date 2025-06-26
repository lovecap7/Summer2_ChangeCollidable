#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateBase abstract
{
protected:
	//プレイヤーのポインタ
	std::weak_ptr<Player> m_player;
	//進行方向を返す関数
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
private:
	//次の状態
	std::shared_ptr<PlayerStateBase> m_nextState;
public:
	PlayerStateBase(std::weak_ptr<Player> player);
	virtual ~PlayerStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<PlayerStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<PlayerStateBase> nextState);
};


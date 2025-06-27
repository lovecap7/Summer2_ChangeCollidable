#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Input;
class Camera;
class Actor;
class ActorManager;
class ActorStateBase abstract
{
public:
	ActorStateBase(std::weak_ptr<Actor> owner);
	virtual ~ActorStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<ActorStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<ActorStateBase> nextState);
	//持ち主の参照
	std::weak_ptr<Actor> GetOwner() const { return m_owner; }
private:
	//次の状態
	std::shared_ptr<ActorStateBase> m_nextState;
protected:
	//自分のポインタ
	std::weak_ptr<Actor> m_owner;
};


#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class PurpleDinosaur;
class Camera;
class ActorManager;
class PurpleDinosaurStateBase abstract
{
protected:
	//自分のポインタ
	std::weak_ptr<PurpleDinosaur> m_owner;
private:
	//次の状態
	std::shared_ptr<PurpleDinosaurStateBase> m_nextState;
public:
	PurpleDinosaurStateBase(std::weak_ptr<PurpleDinosaur> owner);
	virtual ~PurpleDinosaurStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<PurpleDinosaurStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState);
};


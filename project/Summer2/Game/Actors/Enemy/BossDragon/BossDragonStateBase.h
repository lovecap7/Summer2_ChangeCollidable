#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class BossDragon;
class Input;
class Camera;
class AttackBase;
class AttackManager;
class ActorManager;
class BossDragonStateBase abstract
{
protected:
	//自分のポインタ
	std::shared_ptr<BossDragon> m_owner;
	//攻撃を攻撃マネージャーに登録
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//次の状態
	std::shared_ptr<BossDragonStateBase> m_nextState;
public:
	BossDragonStateBase(std::shared_ptr<BossDragon> owner);
	virtual ~BossDragonStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<BossDragonStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<BossDragonStateBase> nextState);
	//プレイヤーを見る
	void LookPlayer();
};




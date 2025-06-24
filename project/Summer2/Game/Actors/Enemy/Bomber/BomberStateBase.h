#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class Bomber;
class Input;
class Camera;
class AttackBase;
class ActorManager;
class AttackManager;
class BomberStateBase abstract
{
protected:
	//自分のポインタ
	std::shared_ptr<Bomber> m_owner;
	//攻撃を攻撃マネージャーに登録
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//次の状態
	std::shared_ptr<BomberStateBase> m_nextState;
public:
	BomberStateBase(std::shared_ptr<Bomber> owner);
	virtual ~BomberStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<BomberStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<BomberStateBase> nextState);
	//モデルをプレイヤーの方向に向かせる
	void LookPlayer();
};

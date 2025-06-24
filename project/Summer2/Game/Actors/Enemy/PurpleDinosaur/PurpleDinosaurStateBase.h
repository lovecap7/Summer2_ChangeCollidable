#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class PurpleDinosaur;
class Input;
class Camera;
class AttackBase;
class AttackManager;
class ActorManager;
class PurpleDinosaurStateBase abstract
{
protected:
	//自分のポインタ
	std::shared_ptr<PurpleDinosaur> m_owner;
	//攻撃を攻撃マネージャーに登録
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//次の状態
	std::shared_ptr<PurpleDinosaurStateBase> m_nextState;
public:
	PurpleDinosaurStateBase(std::shared_ptr<PurpleDinosaur> owner);
	virtual ~PurpleDinosaurStateBase();
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<PurpleDinosaurStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState);
	//モデルをプレイヤーの方向に向かせる
	void LookPlayer();
};


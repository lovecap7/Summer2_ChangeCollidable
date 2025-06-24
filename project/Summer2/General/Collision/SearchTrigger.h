#pragma once
#include "../Math/Vector3.h"
#include <memory>

class Actor;
class Collidable;
class SearchTrigger
{
public:
	SearchTrigger(float radius, float viewAngle,std::shared_ptr<Actor> owner);
	SearchTrigger(float radius ,std::shared_ptr<Actor> owner);
	~SearchTrigger();
	//初期化
	void Init();
	//トリガー
	std::shared_ptr<Collidable> GetCollidable() { return m_serchTrigger; };
	//視野角
	float GetViewAngle() { return m_viewAngle; };
	//前ベクトル
	Vector3 GetViewForward() { return m_forward; };
	void SetViewForward(Vector3 forward) { m_forward = forward; };
	//位置更新
	void SetPos(Vector3 pos);
	//ターゲットを保存
	void OnHitTarget(Vector3 target);
	Vector3 GetTargetPos() { return m_target; };
	bool IsTargetHit() { return m_isHit; };
private:
	//前
	Vector3 m_forward;
	//視野角
	float m_viewAngle;
	//トリガー
	std::shared_ptr<Collidable> m_serchTrigger;
	//索敵に成功したか
	bool m_isHit;
	//ターゲットの座標
	Vector3 m_target;
};


#pragma once
#include "SphereAttackBase.h"
class Actor;
class GameCamera;
class ActorManager;
class Collidable;
class WaveAttack :
    public SphereAttackBase
{
public:
	WaveAttack(std::weak_ptr<Actor> owner);
	virtual ~WaveAttack() {};
	//更新処理
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw() const override;
private:
	//波の大きさ
	float m_waveRadius;
	//波の速度
	float m_waveSpeed;
	//波の中心位置
	Vector3 m_centerPos;
	//波の進行方向
	Vector3 m_waveDir;
	//波の進行フレーム数
	int m_waveFrame;
	//波の最大フレーム数
	int m_maxWaveFrame;
};


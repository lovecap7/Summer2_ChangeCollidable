#pragma once
#include "CameraStateBase.h"
class CameraStateBossDeath :
	public CameraStateBase, public std::enable_shared_from_this<CameraStateBossDeath>
{
public:
	CameraStateBossDeath(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~CameraStateBossDeath() {};
	//初期化処理
	void Init()override;
	//状態に応じた更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


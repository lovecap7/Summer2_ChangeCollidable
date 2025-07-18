#pragma once
#include "CameraStateBase.h"
class CameraStateAreaLock :
    public CameraStateBase, public std::enable_shared_from_this<CameraStateAreaLock>
{
public:
	CameraStateAreaLock(std::weak_ptr<Camera> camera);
	virtual ~CameraStateAreaLock() {};
	//初期化処理
	void Init()override;
	//状態に応じた更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


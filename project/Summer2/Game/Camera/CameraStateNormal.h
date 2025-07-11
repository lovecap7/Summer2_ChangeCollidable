#pragma once
#include "CameraStateBase.h"
class CameraStateNormal :
    public CameraStateBase, public std::enable_shared_from_this<CameraStateNormal>
{
public:
	CameraStateNormal(std::weak_ptr<Camera> camera);
	virtual ~CameraStateNormal() {};
	//初期化処理
	void Init()override;
	//状態に応じた更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


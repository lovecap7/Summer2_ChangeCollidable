#pragma once
#include "CameraStateBase.h"
class CameraStateBossDeath :
	public CameraStateBase, public std::enable_shared_from_this<CameraStateBossDeath>
{
public:
	CameraStateBossDeath(std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~CameraStateBossDeath() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


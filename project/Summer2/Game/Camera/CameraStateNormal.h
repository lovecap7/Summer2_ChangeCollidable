#pragma once
#include "CameraStateBase.h"
class CameraStateNormal :
    public CameraStateBase, public std::enable_shared_from_this<CameraStateNormal>
{
public:
	CameraStateNormal(std::weak_ptr<Camera> camera);
	virtual ~CameraStateNormal() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


#pragma once
#include "CameraStateBase.h"
class CameraStateBossArea :
	public CameraStateBase, public std::enable_shared_from_this<CameraStateBossArea>
{
public:
	CameraStateBossArea(std::weak_ptr<Camera> camera);
	virtual ~CameraStateBossArea() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};


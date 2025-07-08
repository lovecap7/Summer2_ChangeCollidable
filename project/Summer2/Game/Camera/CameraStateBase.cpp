#include "CameraStateBase.h"
CameraStateBase::CameraStateBase(std::weak_ptr<Camera> camera):
	m_camera(camera)
{
}

void CameraStateBase::ChangeState(std::shared_ptr<CameraStateBase> nextState)
{
	m_nextState = nextState;
}

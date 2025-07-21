#include "GameCameraStateBase.h"
GameCameraStateBase::GameCameraStateBase(std::weak_ptr<GameCamera> camera):
	m_camera(camera)
{
}

void GameCameraStateBase::ChangeState(std::shared_ptr<GameCameraStateBase> nextState)
{
	m_nextState = nextState;
}

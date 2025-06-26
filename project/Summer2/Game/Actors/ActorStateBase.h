#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Input;
class Camera;
class ActorManager;
class ActorStateBase abstract
{
public:
	ActorStateBase();
	virtual ~ActorStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<ActorStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<ActorStateBase> nextState);
private:
	//���̏��
	std::shared_ptr<ActorStateBase> m_nextState;
};


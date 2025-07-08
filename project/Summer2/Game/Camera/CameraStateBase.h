#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Camera;
class ActorManager;
class CameraStateBase abstract
{
public:
	CameraStateBase(std::weak_ptr<Camera> camera);
	virtual ~CameraStateBase() {};
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<CameraStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<CameraStateBase> nextState);
private:
	//�J�����̏��
	std::shared_ptr<CameraStateBase> m_nextState;
protected:
	//�J�����̎�����
	std::weak_ptr<Camera> m_camera;
};


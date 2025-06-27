#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
class Input;
class Camera;
class Actor;
class ActorManager;
class CharacterStateBase abstract
{
public:
	CharacterStateBase(std::weak_ptr<Actor> owner);
	virtual ~CharacterStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<CharacterStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<CharacterStateBase> nextState);
	//������̎Q��
	std::weak_ptr<Actor> GetOwner() const { return m_owner; }
private:
	//���̏��
	std::shared_ptr<CharacterStateBase> m_nextState;
protected:
	//�����̃|�C���^
	std::weak_ptr<Actor> m_owner;
};


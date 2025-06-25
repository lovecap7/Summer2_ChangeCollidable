#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class PurpleDinosaur;
class Input;
class Camera;
class AttackBase;
class AttackManager;
class ActorManager;
class PurpleDinosaurStateBase abstract
{
protected:
	//�����̃|�C���^
	std::weak_ptr<PurpleDinosaur> m_owner;
private:
	//���̏��
	std::shared_ptr<PurpleDinosaurStateBase> m_nextState;
public:
	PurpleDinosaurStateBase(std::weak_ptr<PurpleDinosaur> owner);
	virtual ~PurpleDinosaurStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<Camera> camera) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<PurpleDinosaurStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState);
	//���f�����v���C���[�̕����Ɍ�������
	void LookPlayer();
};


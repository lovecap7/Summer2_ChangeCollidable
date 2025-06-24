#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class SmallDragon;
class Input;
class Camera;
class AttackBase;
class AttackManager;
class ActorManager;
class SmallDragonStateBase abstract
{
protected:
	//�����̃|�C���^
	std::shared_ptr<SmallDragon> m_owner;
	//�U�����U���}�l�[�W���[�ɓo�^
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//���̏��
	std::shared_ptr<SmallDragonStateBase> m_nextState;
public:
	SmallDragonStateBase(std::shared_ptr<SmallDragon> owner);
	virtual ~SmallDragonStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<SmallDragonStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<SmallDragonStateBase> nextState);
	//�v���C���[������
	void LookPlayer();
};


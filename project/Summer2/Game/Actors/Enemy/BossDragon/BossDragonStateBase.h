#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class BossDragon;
class Input;
class Camera;
class AttackBase;
class AttackManager;
class ActorManager;
class BossDragonStateBase abstract
{
protected:
	//�����̃|�C���^
	std::shared_ptr<BossDragon> m_owner;
	//�U�����U���}�l�[�W���[�ɓo�^
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//���̏��
	std::shared_ptr<BossDragonStateBase> m_nextState;
public:
	BossDragonStateBase(std::shared_ptr<BossDragon> owner);
	virtual ~BossDragonStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<BossDragonStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<BossDragonStateBase> nextState);
	//�v���C���[������
	void LookPlayer();
};




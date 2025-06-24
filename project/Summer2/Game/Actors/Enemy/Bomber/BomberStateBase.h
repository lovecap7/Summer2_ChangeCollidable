#pragma once
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class Bomber;
class Input;
class Camera;
class AttackBase;
class ActorManager;
class AttackManager;
class BomberStateBase abstract
{
protected:
	//�����̃|�C���^
	std::shared_ptr<Bomber> m_owner;
	//�U�����U���}�l�[�W���[�ɓo�^
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//���̏��
	std::shared_ptr<BomberStateBase> m_nextState;
public:
	BomberStateBase(std::shared_ptr<Bomber> owner);
	virtual ~BomberStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<BomberStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<BomberStateBase> nextState);
	//���f�����v���C���[�̕����Ɍ�������
	void LookPlayer();
};

#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
class Player;
class Input;
class Camera;
class AttackManager;
class ActorManager;
class AttackBase;
class PlayerStateBase abstract
{
protected:
	//�v���C���[�̃|�C���^
	std::shared_ptr<Player> m_player;
	//�i�s������Ԃ��֐�
	virtual Vector3 GetForwardVec(const Input& input, const std::unique_ptr<Camera>& camera);
	//�U�����U���}�l�[�W���[�ɓo�^
	virtual void AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager);
private:
	//���̏��
	std::shared_ptr<PlayerStateBase> m_nextState;
public:
	PlayerStateBase(std::shared_ptr<Player> player);
	virtual ~PlayerStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<PlayerStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<PlayerStateBase> nextState);
};


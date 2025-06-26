#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateBase abstract
{
protected:
	//�v���C���[�̃|�C���^
	std::weak_ptr<Player> m_player;
	//�i�s������Ԃ��֐�
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
private:
	//���̏��
	std::shared_ptr<PlayerStateBase> m_nextState;
public:
	PlayerStateBase(std::weak_ptr<Player> player);
	virtual ~PlayerStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<PlayerStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<PlayerStateBase> nextState);
};


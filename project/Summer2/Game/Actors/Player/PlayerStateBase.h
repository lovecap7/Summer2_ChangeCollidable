#pragma once
#include "../ActorStateBase.h"
#include <memory>
#include "../../../General/Math/MyMath.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateBase abstract:
	public ActorStateBase
{
protected:
	//�v���C���[�̃|�C���^
	std::weak_ptr<Player> m_player;
	//�i�s������Ԃ��֐�
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
public:
	PlayerStateBase(std::weak_ptr<Player> player);
	virtual ~PlayerStateBase();
};


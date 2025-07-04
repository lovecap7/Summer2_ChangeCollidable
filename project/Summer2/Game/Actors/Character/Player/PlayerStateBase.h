#pragma once
#include "../CharacterStateBase.h"
#include <memory>
#include "../../../../General/Math/MyMath.h"
#include <string>
class Actor;
class Slash;
class Camera;
class ActorManager;
class PlayerStateBase abstract:
	public CharacterStateBase
{
public:
	PlayerStateBase(std::weak_ptr<Actor> player);
	virtual ~PlayerStateBase();
protected:
	//�i�s������Ԃ��֐�
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
};

//�U���̃f�[�^����������Ƃ��Ɏg��
namespace
{
	std::string kPlayerName = "Player";
	std::string kNA1Name	= "NA1";
	std::string kNA2Name	= "NA2";
	std::string kNA3Name	= "NA3";
}


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
	//進行方向を返す関数
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
};

//攻撃のデータを検索するときに使う
namespace
{
	std::string kPlayerName = "Player";
	std::string kNA1Name	= "NA1";
	std::string kNA2Name	= "NA2";
	std::string kNA3Name	= "NA3";
}


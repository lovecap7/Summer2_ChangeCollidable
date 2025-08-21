#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
//アクターが索敵場所の中にいるかチェックする
class Actor;
class SearchPlace 
{
public:
	SearchPlace(std::weak_ptr<Actor> target,float rang);
	~SearchPlace();
	//デバッグ用
	void Draw()const;
	//範囲内にいるか
	bool IsInSearchPlace()const;
	Vector3 GetPos()const { return m_pos; };
private:
	//チェックするアクター
	std::weak_ptr<Actor> m_target;
	//範囲
	float m_rang;
	//座標
	Vector3 m_pos;
};


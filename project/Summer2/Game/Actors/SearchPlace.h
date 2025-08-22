#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
//アクターが索敵場所の中にいるかチェックする
class SearchPlace 
{
public:
	SearchPlace(Vector3 pos,float rang);
	~SearchPlace();
	//デバッグ用
	void Draw()const;
	//範囲内にいるか
	bool IsInSearchPlace(Vector3 targetPos)const;
	//座標
	Vector3 GetPos()const { return m_pos; };
	//範囲
	void SetRang(float rang) { m_rang = rang; };
	float GetRang() const{ return m_rang; };
private:
	//範囲
	float m_rang;
	//座標
	Vector3 m_pos;
};


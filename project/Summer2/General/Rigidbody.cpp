#include "Rigidbody.h"

Rigidbody::Rigidbody():
	m_pos(),
	m_vec(),
	m_isGravity(true) //重力を受ける
{
}


Vector3 Rigidbody::GetMoveVec()
{
	Vector3 moveVec = m_vec;
	moveVec.y = 0.0f; //Y軸は無視
	//X,Zのベクトルを返す
	if (moveVec.Magnitude() <= 0.0f)
	{
		//移動ベクトルが0なら0を返す
		moveVec = Vector3{ 0.0f,0.0f,0.0f };
	}
	
	return moveVec;
}

void Rigidbody::SetMoveVec(Vector3 vec)
{
	m_vec.x = vec.x;
	m_vec.z = vec.z;
}

void Rigidbody::SpeedDown(float decRate)
{
	//減速
	m_vec.x *= decRate;
	m_vec.z *= decRate;
}

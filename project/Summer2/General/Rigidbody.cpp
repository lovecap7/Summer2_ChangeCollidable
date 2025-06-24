#include "Rigidbody.h"

Rigidbody::Rigidbody():
	m_pos(),
	m_vec()
{
}

Rigidbody::Rigidbody(Vector3 pos):
	m_pos(pos),
	m_vec()
{
}

Vector3 Rigidbody::GetMoveVec()
{
	Vector3 moveVec = m_vec;
	moveVec.y = 0.0f; //Y���͖���
	//X,Z�̃x�N�g����Ԃ�
	if (moveVec.Magnitude() <= 0.0f)
	{
		//�ړ��x�N�g����0�Ȃ�0��Ԃ�
		moveVec = Vector3{ 0.0f,0.0f,0.0f };
	}
	
	return moveVec;
}

void Rigidbody::SetMoveVec(Vector3 vec)
{
	m_vec.x = vec.x;
	m_vec.z = vec.z;
}

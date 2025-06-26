#pragma once
#include "Math/Vector3.h"
class Rigidbody
{
public:
	Rigidbody();
	virtual ~Rigidbody() {};
	//�ʒu
	Vector3 GetPos() { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	void SetPosX(float x) { m_pos.x = x; };
	void SetPosY(float y) { m_pos.y = y; };
	void SetPosZ(float z) { m_pos.z = z; };
	//�x�N�g��
	Vector3 GetVec() { return m_vec; };
	Vector3 GetMoveVec();//���������̃x�N�g��
	void SetVec(Vector3 vec) { m_vec = vec; };
	void SetVecX(float x) { m_vec.x = x; };
	void SetVecY(float y) { m_vec.y = y; };
	void SetVecZ(float z) { m_vec.z = z; };
	void AddVec(Vector3 vec) { m_vec += vec; };
	void SetMoveVec(Vector3 vec);
	void ResetVec() { m_vec = m_vec.Zero(); };
	//���̍��W
	Vector3 GetNextPos() { return m_pos + m_vec; };
	//�X�s�[�h�_�E��
	void SpeedDown(float decRate);
public:
	//���W�ƃx�N�g��
	Vector3 m_pos;
	Vector3 m_vec;
	//�d�͂��󂯂�
	bool m_isGravity ; //�d�͂��󂯂邩�ǂ���
};


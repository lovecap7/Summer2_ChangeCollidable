#pragma once
#include "Math/MyMath.h"
class MyEffect
{
public:
	MyEffect(int playHandle, Vector3 pos);
	~MyEffect();
	void Update();					//�X�V
	void Play();					//�Đ�
	void Stop();					//��~
	void SetPlaySpeed(float speed);	//�Đ����x
	void Delete();					//�폜

	void SetPos(Vector3 pos);		//���W
	void SetRot(Vector3 rot);		//��]
	void SetScale(Vector3 scale);		//�g��
private:
	//�Đ��n���h��
	int m_playHandle;
	//���W
	Vector3 m_pos;
	//��]
	Vector3 m_rot;
	//�g��
	Vector3 m_scale;
	//�Đ����x
	float m_playSpeed;
	//�폜�t���O
	bool m_isDelete;
};


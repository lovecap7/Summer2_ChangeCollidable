#pragma once
#include "../Math/MyMath.h"
class MyEffect
{
public:
	MyEffect(int playHandle, Vector3 pos);
	virtual ~MyEffect();
	virtual void Update();			//�X�V
	virtual void End();				//�I������
	void Play();					//�Đ�
	void Stop();					//��~
	void SetPlaySpeed(float speed);	//�Đ����x
	void Delete();					//�폜
	bool IsDelete() const { return m_isDelete; };
	//�Z�b�^�[
	void SetPos(Vector3 pos);		//���W
	void SetRot(Vector3 rot);		//��]
	void SetScale(Vector3 scale);	//�g��
	void LookAt(Vector3 dir);		//������������

	//�w��t���[���Đ�
	void SpecificFrame(int frame);
protected:
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
	//�w��t���[����ɍ폜
	int m_specificFrame;
	bool m_isSpecificFrame;
	//�G�t�F�N�g�̌���(Z����)
	Vector3 m_dir;
};


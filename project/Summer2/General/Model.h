#pragma once
#include <DxLib.h>
#include <memory>
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

class Animator;
class Model
{
public:
	Model(int modelHandle, VECTOR pos);
	Model(int modelHandle, VECTOR pos, Vector3 dir);
	virtual ~Model();
	int GetModelHandle() { return m_modelHandle; };
	void Update();
	void Draw()const;
	//�Z�b�^�[
	void SetPos(VECTOR pos);
	void SetScale(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetDir(Vector2 vec);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDiffuseColor(COLOR_F color);
	//�Q�b�^�[
	Vector3 GetPos() const { return m_pos; };
	Vector3 GetScale() const { return m_scale; };
	//����
	Vector3 GetDir();
	//�U�������������̃��f���̃��A�N�V����(�U�������������ɌĂ�)
	void ModelHit();
	//�A�j���[�V�����֌W
	void SetAnim(const char* animName,bool isLoop);//�A�j���[�V�����ύX
	void SetAnim(const char* animName,bool isLoop,const float& animSpeed);//�A�j���[�V�����ύX(�Đ����x��)
	void SetAnimSpeed(const float& animSpeed);//�A�j���[�V�����̑��x�ύX
	bool IsFinishAnim();//�A�j���[�V�������I��������ǂ���
	float GetTotalAnimFrame();//�S�̃t���[��
	void SetTotalAnimFrame(float totalFrame);//�S�̃t���[���Z�b�g
	float GetNowAnimFrame();//���݂̃A�j���[�V�����t���[��
	void ReplayAnim();//�A�j���[�V�������ŏ�����Đ�
	void DeleteAnim();//�Đ����̃A�j���[�V�������폜
	void SetFixedLoopFrame(float loopFrame);//�w�胋�[�v�Đ�
	bool IsFinishFixedLoop();//�w�胋�[�v�Đ��I��
private:
	int m_modelHandle;
	//�A�j���[�V����
	std::unique_ptr<Animator> m_animator;
	//��]�̌���
	Vector3 m_angleAxis;
	//���f���̉�]���
	Quaternion m_rotation;
	//���f���̐���
	Vector3 m_forward;
	//���̃��f���̐���
	Vector3 m_nextForward;
	//��]�N�H�[�^�j�I��
	Quaternion m_rotaQ;
	//��]�t���[��
	int m_rotaFrame;
	//�����̍��W
	Vector3 m_pos;
	//�傫��
	Vector3 m_scale;

	//�U�������������ɂ��Ƃ̏�Ԃɖ߂�̂𐔂���t���[��
	int m_hitCountFrame;
	//�U�������������ɐԐF���猳�ɖ߂������Ɏg��
	COLOR_F m_diffColor;
	//�U����H��������ɏ����傫������̂ō����o���Ă���
	Vector3 m_beforeScaleDif;
	Vector3 m_beforeScale;
};


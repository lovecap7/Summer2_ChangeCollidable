#pragma once
#include "../Math/MyMath.h"
class Quaternion
{
public:
	//�ϐ�
	float x, y, z, w;
	//�R���X�g���N�^
	Quaternion();
	Quaternion(float w,float x, float y, float z);
	Quaternion(const Quaternion& q);

	//���Z�q�I�[�o�[���[�h
	//�N�H�[�^�j�I�����m
	Quaternion operator *(const Quaternion& q)const;
	//�N�H�[�^�j�I���ƃx�N�^�[
	Vector3 operator *(const Vector3& v)const;
	//�N�H�[�^�j�I���ƃX�J���[
	Quaternion operator *(const float& s)const;
	//�N�I�[�^�j�I�����m�̑����Z
	Quaternion operator +(const Quaternion& q)const;
	//�N�H�[�^�j�I�����m�̔�r
	bool operator ==(const Quaternion& q)const;
	bool operator !=(const Quaternion& q)const;
	//����
	float DotQ(const Quaternion& q)const;
	//�N�H�[�^�j�I���𐳋K��
	Quaternion NormQ()const;
	//�P�ʃN�H�[�^�j�I��
	static Quaternion IdentityQ();
	//����(�t)�N�H�[�^�j�I��
	Quaternion InverseQ()const;
	//�傫��
	float Magnitude()const;
	//���̑傫��
	float AxisMagnitude() const;

	/// <summary>
	/// �I�C���[�p����N�H�[�^�j�I�����쐬
	/// </summary>
	/// <param name="x">���[��(�x���@)</param>
	/// <param name="y">���[(�x���@)</param>
	/// <param name="z">�s�b�`(�x���@)</param>
	/// <returns>��]�N�I�[�^�j�I��</returns>
	static Quaternion Euler(float xDeg, float yDeg, float zDeg);
	/// <summary>
	/// �}�g���N�X�ɕϊ�
	/// </summary>
	/// <param name="q">�N�H�[�^�j�I��</param>
	/// <returns>�}�g���N�X</returns>
	Matrix4x4 GetMatrix() const;
	/// <summary>
	/// ��]�N�H�[�^�j�I��
	/// </summary>
	/// <param name="rad">���W�A��</param>
	/// <param name="axis">��</param>
	/// <returns>��]�N�H�[�^�j�I��</returns>
	static Quaternion AngleAxis(const float& rad, const Vector3& axis);
	/// <summary>
	/// ���ʐ��`��ԁiSpherical Linear Interpolation�j
	/// </summary>
	/// <param name="sRota">�J�n�̉�]�N�I�[�^�j�I��</param>
	/// <param name="eRota">�I���̉�]�N�I�[�^�j�I��</param>
	/// <param name="t">��ԌW��(0~1)</param>
	/// <returns>��Ԍ�N�I�[�^�j�I��</returns>
	static Quaternion Slerp(const Quaternion& sRota, const Quaternion& eRota, float t);
	
};


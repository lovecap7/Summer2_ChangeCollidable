#pragma once
#include <DxLib.h>

class Vector2;
class Vector3
{
public:
	Vector3();
	Vector3(float initX, float initY, float initZ);
	Vector3(VECTOR vec);
	~Vector3() {};
	float x, y, z;

	/// <summary>
	/// �x�N�g���̑傫����Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	float Magnitude()const;

	/// <summary>
	/// �x�N�g���̑傫����2���Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	float SqMagnitude() const;

	/// <summary>
	/// ���K��(�傫����1�ɂ���)
	/// </summary>
	Vector3 Normalize() const;

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	float Dot(const Vector3& right)const;

	/// <summary>
	/// �O��
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& right)const;

	/// <summary>
	/// DxLib�̃x�N�g���ɕϊ����܂�
	/// </summary>
	/// <returns></returns>
	VECTOR ToDxLibVector()const;

	/// <summary>
	/// Lerp(���`���)���s���܂��B
	/// </summary>
	/// <param name="start">�J�n�ʒu(���W)</param>
	/// <param name="end">�I���ʒu(���W)</param>
	/// <param name="t">��Ԃ̒l</param>
	/// <returns>��Ԍ�̃x�N�g��(���W)</returns>
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

	/// <summary>
	/// XZ�̐��������ׂ��Q��Ԃ�
	/// </summary>
	/// <returns></returns>
	Vector2 XZ();
	
	/// <summary>
	/// Slerp(���ʐ��`���)���s���܂��B
	/// </summary>
	/// <param name="start">�J�n�ʒu(���W)</param>
	/// <param name="end">�I���ʒu(���W)</param>
	/// <param name="t">��Ԃ̒l</param>
	/// <returns>��Ԍ�̃x�N�g��(���W)</returns>
	//static Vector3 Slerp(const Vector3& start, const Vector3& end, float t);

	//���Z�q�I�[�o�[���[�h
	Vector3 operator+(const Vector3& right)const;
	Vector3 operator-(const Vector3& right)const;
	Vector3 operator*(const float& right)const;
	Vector3 operator/(const float& right)const;
	Vector3 operator=(const float& right)const;
	Vector3 operator+=(const Vector3& right);
	Vector3 operator-=(const Vector3& right);
	Vector3 operator*=(const float& right);
	Vector3 operator/=(const float& right);
	bool operator==(const Vector3& right)const;
	bool operator!=(const Vector3& right)const;

	//���x�N�g��
	static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); };
	static Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); };
	static Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); };
	static Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); };
	static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); };
	static Vector3 Back() { return Vector3(0.0f, 0.0f, -1.0f); };
	/// <summary>
	/// 0�x�N�g���ɂ���
	/// </summary>
	static Vector3 Zero();
	

};
//���W�ŕ\���Ƃ��Ɏg��
typedef Vector3 Position3;

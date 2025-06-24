#pragma once
#include <cmath>
class Vector3;
class Vector2
{
public:
	/// <summary>
	/// 2D�p
	/// </summary>
	Vector2() :x(0.0f), y(0.0f) {};
	Vector2(float initX, float initY) :x(initX), y(initY) {};
	float x, y;
	/// <summary>
	/// �x�N�g���̑傫����Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	float Magnitude()const;

	/// <summary>
	/// ���K��(�傫����1�ɂ���)
	/// </summary>
	Vector2 Normalize() const;

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	float Dot(const Vector2& right)const;

	/// <summary>
	/// �O��
	/// </summary>
	/// <param name="right"></param>
	/// <returns>�X�J���[</returns>
	float Cross(const Vector2& right)const;

	/// <summary>
	/// 0�x�N�g���ɂ���
	/// </summary>
	Vector2 Zero()const;

	//���Z�q�I�[�o�[���[�h
	Vector2 operator+(const Vector2& right)const;
	Vector2 operator-(const Vector2& right)const;
	Vector2 operator*(const float& right)const;
	Vector2 operator/(const float& right)const;
	Vector2 operator+=(const Vector2& right);
	Vector2 operator-=(const Vector2& right);
	Vector2 operator*=(const float& right);
	Vector2 operator/=(const float& right);
	bool operator==(const Vector2& right)const;
	bool operator!=(const Vector2& right)const;

	static float Theata(Vector2 vec1, Vector2 vec2);

	Vector3 XZ();
};

//���W�ŕ\���Ƃ��Ɏg��
typedef Vector2 Position2;


#pragma once
#include "Vector2.h"
#include <cmath>
class Matrix3x3
{
public:
	Matrix3x3();
	~Matrix3x3() {};

	float mat[3][3];
	/// <summary>
	/// �Q�̍s��̏�Z��Ԃ�(��Z�̏��Ԃɒ���)
	/// </summary>
	/// <param name="left">���Ӎs��</param>
	/// <param name="right">�E�Ӎs��</param>
	/// <returns></returns>
	Matrix3x3  MultipleMat3x3(const Matrix3x3& left, const Matrix3x3& right);
	Matrix3x3 operator* (const Matrix3x3& right);

	/// <summary>
	/// �x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
	/// </summary>
	/// <param name="m">�s��</param>
	/// <param name="v">�x�N�g��</param>
	/// <returns></returns>
	Vector2 MultipleVec2(const Matrix3x3& m, const Vector2& v);
	Vector2 operator*(const Vector2& v);

};

/// <summary>
/// /�P�ʍs���Ԃ�(3x3)
/// </summary>
/// <returns></returns>
Matrix3x3 IdentityMat3x3();

/// <summary>
/// ���s�ړ��s���Ԃ�(3x3)
/// </summary>
/// <param name="x">X�����̈ړ���</param>
/// <param name="y">Y�����̈ړ���</param>
/// <returns></returns>
Matrix3x3 TranslateMat3x3(float x, float y);

/// <summary>
/// ��]�s���Ԃ�(3x3)
/// </summary>
/// <param name="angle">��]�p�x</param>
/// <returns></returns>
Matrix3x3 RotateMat3x3(float angle);

/// <summary>
/// �Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂�
/// </summary>
/// <param name="center">��]���S�_</param>
/// <param name="angle">��]�p�x</param>
/// <returns></returns>
Matrix3x3 RotatePositionMatrix3x3(const Vector2& center, float angle);


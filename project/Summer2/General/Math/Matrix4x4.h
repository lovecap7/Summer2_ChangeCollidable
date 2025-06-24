#pragma once
#include "Vector3.h"
class Matrix4x4
{
public:
	Matrix4x4();
	~Matrix4x4() {};

	float mat[4][4];

	/// <summary>
	/// �Q�̍s��̏�Z��Ԃ�(��Z�̏��Ԃɒ���)
	/// </summary>
	/// <param name="left">���Ӎs��</param>
	/// <param name="right">�E�Ӎs��</param>
	/// <returns></returns>
	Matrix4x4 MultipleMat4x4(const Matrix4x4& left, const Matrix4x4& right);
	Matrix4x4 operator* (const Matrix4x4& right);

	/// <summary>
	/// �x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
	/// </summary>
	/// <param name="m">�s��</param>
	/// <param name="v">�x�N�g��</param>
	/// <returns></returns>
	Vector3 MultipleVec3(const Matrix4x4& m, const Vector3& v);
	Vector3 operator*(const Vector3& v);

	/// <summary>
	/// /�P�ʍs���Ԃ�(4x4)
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 IdentityMat4x4();
	
	/// <summary>
	/// ���s�ړ��s���Ԃ�(4x4)
	/// </summary>
	/// <param name="x">X�����̈ړ���</param>
	/// <param name="y">Y�����̈ړ���</param>
	/// <param name="z">Z�����̈ړ���</param>
	/// <returns></returns>
	static Matrix4x4 TranslateMat4x4(float x, float y, float z);

	/// <summary>
	/// X����]�s���Ԃ�(4x4)
	/// </summary>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateXMat4x4(float angle);

	/// <summary>
	/// Y����]�s���Ԃ�(4x4)
	/// </summary>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateYMat4x4(float angle);

	/// <summary>
	/// Z����]�s���Ԃ�(4x4)
	/// </summary>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateZMat4x4(float angle);


	/// <summary>
	/// �Ƃ���_�𒆐S��Y����]���āA���̉�]��̍��W��Ԃ��܂�
	/// </summary>
	/// <param name="center">��]���S�_</param>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateYPositionMatrix4x4(const Position3& center, float angle);

	/// <summary>
	/// �Ƃ���_�𒆐S��X����]���āA���̉�]��̍��W��Ԃ��܂�
	/// </summary>
	/// <param name="center">��]���S�_</param>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateXPositionMatrix4x4(const Position3& center, float angle);

	/// <summary>
	/// �Ƃ���_�𒆐S��Z����]���āA���̉�]��̍��W��Ԃ��܂�
	/// </summary>
	/// <param name="center">��]���S�_</param>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateZPositionMatrix4x4(const Position3& center, float angle);

	/// <summary>
	/// �Ƃ���_�𒆐S��X����]����Y����]���āA���̉�]��̍��W��Ԃ��܂�
	/// </summary>
	/// <param name="center">��]���S�_</param>
	/// <param name="hAngle">��]�p�x</param>
	/// <param name="vAngle">��]�p�x</param>
	/// <returns></returns>
	static Matrix4x4 RotateXYPositionMatrix4x4(const Position3& center, float hAngle, float vAngle);

	/// <summary>
	/// �傫���̍s��
	/// </summary>
	/// <param name="scele"></param>
	/// <returns></returns>
	static Matrix4x4 ScaleMatrix4x4(const Vector3& scele);

	/// <summary>
	/// DX���C�u�����̍s��ɕϊ�
	/// </summary>
	/// <returns></returns>
	MATRIX ToDxLibMATRIX();

	/// <summary>
	/// DX���C�u�����̍s��ɕϊ�
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 ToMatrix4x4(MATRIX dxMat);

	/// <summary>
	/// ��������������������]�s���Ԃ�
	/// </summary>
	/// <param name="target">������������</param>
	/// <param name="up">�����X���Ă��鎞�ɒl��n��</param>
	/// <returns>��]�s��</returns>
	static Matrix4x4 LookAt(const Vector3& target,const Vector3 up = Vector3::Up());
};

#include "Quaternion.h"
#include <cmath>
#include <cassert>
Quaternion::Quaternion():
	w(1.0f),
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Quaternion::Quaternion(float w,float x, float y, float z):
	w(w),
	x(x),
	y(y),
	z(z)
{
}
Quaternion::Quaternion(const Quaternion& q) :
	w(q.w),
	x(q.x),
	y(q.y),
	z(q.z)
{
}
//�N�H�[�^�j�I���̉�]�̍���
Quaternion Quaternion::operator*(const Quaternion& q)const
{
	//�N�H�[�^�j�I���̊|���Z
	Quaternion rQ;
	rQ.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;//����
	rQ.x = this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y;//����
	rQ.y = this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z;//����
	rQ.z = this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x;//����
	return rQ;
}
//�x�N�g����]������
Vector3 Quaternion::operator*(const Vector3& v)const
{
	Quaternion qPos, qInv;
	Vector3 vPos;
	//�O�������W���N�H�[�^�j�I���ɕϊ�
	qPos.w = 1.0f;
	qPos.x = v.x;
	qPos.y = v.y;
	qPos.z = v.z;

	//��]�N�H�[�^�j�I���̃C���o�[�X���쐬
	qInv = this->InverseQ();
	//��]�N�H�[�^�j�I���̍쐬
	qPos = *this * qPos * qInv;
	//�O�������W�ɖ߂�
	vPos.x = qPos.x;
	vPos.y = qPos.y;
	vPos.z = qPos.z;

	return vPos;
}
Quaternion Quaternion::operator*(const float& s)const
{
	Quaternion rQ;
	rQ.w = this->w * s;
	rQ.x = this->x * s;
	rQ.y = this->y * s;
	rQ.z = this->z * s;
	return rQ;
}
Quaternion Quaternion::operator+(const Quaternion& q) const
{
	Quaternion rQ;
	rQ.w = this->w + q.w;
	rQ.x = this->x + q.x;
	rQ.y = this->y + q.y;
	rQ.z = this->z + q.z;
	return rQ;
}
bool Quaternion::operator==(const Quaternion& q) const
{
	return (this->w == q.w && this->x == q.x && this->y == q.y && this->z == q.z);
}
bool Quaternion::operator!=(const Quaternion& q) const
{
	return !(*this == q);
}
//��̃N�H�[�^�j�I���̊Ԃ̊p�x�����߂�ۂɎg��
float Quaternion::DotQ(const Quaternion& q)const
{
	float result = this->w * q.w + this->x * q.x + this->y * q.y + this->z * q.z;
	return result;
}

//���K���N�H�[�^�j�I��(�l�̌덷���Ȃ���)
Quaternion Quaternion::NormQ()const
{
	//�P�ʃN�H�[�^�j�I��
	Quaternion rQ = IdentityQ();
	//�������邩�`�F�b�N
	if (this->AxisMagnitude() <= 0.0f)
	{
		return rQ;//��]���Ȃ�
	}
	//�����̑傫�����o��(��Βl)
	float length = this->Magnitude();
	//�����Ɏ����̃C���o�[�X�������Đ��K��
	//rQ = *this * this->InverseQ();
	rQ.w = this->w / length;
	rQ.x = this->x / length;
	rQ.y = this->y / length;
	rQ.z = this->z / length;
	return rQ;
}
//��]���Ȃ��N�H�[�^�j�I��(�P�ʃN�H�[�^�j�I��)
Quaternion Quaternion::IdentityQ()
{
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

//����(�t)�N�H�[�^�j�I��
Quaternion Quaternion::InverseQ()const
{
	return  Quaternion(this->w, -this->x, -this->y, -this->z);
}

float Quaternion::Magnitude() const
{
	float magnitude = sqrt((this->w * this->w) + (this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	return magnitude;
}


float Quaternion::AxisMagnitude() const
{
	float magnitude = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	return magnitude;
}

Quaternion Quaternion::Euler(float xDeg, float yDeg, float zDeg)
{
	//���W�A���ɕϊ�
	float xRad = xDeg * MyMath::DEG_2_RAD;
	float yRad = yDeg * MyMath::DEG_2_RAD;
	float zRad = zDeg * MyMath::DEG_2_RAD;

	//�P�ʃN�H�[�^�j�I��
	Quaternion rQ = IdentityQ();
	//�e�v�f�̉�]�N�H�[�^�j�I�����쐬
	Vector3 right = { 1.0f,0.0f,0.0f };
	Quaternion xQ = AngleAxis(xRad, right);//���[��
	Vector3 up = { 0.0f,1.0f,0.0f };
	Quaternion yQ = AngleAxis(yRad, up);//���[
	Vector3 forward = { 0.0f,0.0f,-1.0f };
	Quaternion zQ = AngleAxis(zRad, forward);//�s�b�`
	//�v�Z
	rQ = yQ * xQ * zQ;//Unity�̉�]�����ɍ��킹��
	return rQ.NormQ();
}

Matrix4x4 Quaternion::GetMatrix() const
{
	Matrix4x4 rM = Matrix4x4::IdentityMat4x4();
	Quaternion q = *this;
	if (q.Magnitude() > 0.0f)
	{
		q = q.NormQ();
	}
	// �N�I�[�^�j�I���̊e�������g���čs��̗v�f���v�Z
	auto xy2 = q.x * q.y * 2;
	auto xz2 = q.x * q.z * 2;
	auto xw2 = q.x * q.w * 2;
	auto yz2 = q.y * q.z * 2;
	auto yw2 = q.y * q.w * 2;
	auto zw2 = q.z * q.w * 2;
	auto ww2 = q.w * q.w * 2;

	// �s��̊e�v�f���v�Z
	auto r00 = ww2 + 2 * q.x * q.x - 1;
	auto r01 = xy2 + zw2;
	auto r02 = xz2 - yw2;

	auto r10 = xy2 - zw2;
	auto r11 = ww2 + 2 * q.y * q.y - 1;
	auto r12 = yz2 + xw2;

	auto r20 = xz2 + yw2;
	auto r21 = yz2 - xw2;
	auto r22 = ww2 + 2 * q.z * q.z - 1;

	// �v�Z�����v�f���s��ɐݒ�
	rM.mat[0][0] = r00;
	rM.mat[0][1] = r01;
	rM.mat[0][2] = r02;
	rM.mat[1][0] = r10;
	rM.mat[1][1] = r11;
	rM.mat[1][2] = r12;
	rM.mat[2][0] = r20;
	rM.mat[2][1] = r21;
	rM.mat[2][2] = r22;
	return rM;
}

Quaternion Quaternion::AngleAxis(const float& rad, const Vector3& axis)
{
	Quaternion rQ = Quaternion::IdentityQ();
	if (axis.Magnitude() <= 0.0f)
	{
		return rQ;
	}

	//���K��
	Vector3 nAxis = axis.Normalize();
	rQ.w = cos(rad / 2.0f);//����
	rQ.x = nAxis.x * sin(rad / 2.0f);//����
	rQ.y = nAxis.y * sin(rad / 2.0f);//����
	rQ.z = nAxis.z * sin(rad / 2.0f);//����
	return rQ;
}

//��̌����E��]�̊Ԃ����p���x�Ŋ��炩�ɕۊǂ���
//��]������𒼐��ł͂Ȃ����ʏ�̍ŒZ��(��~)�ŕ⊮����
Quaternion Quaternion::Slerp(const Quaternion& sRota, const Quaternion& eRota, float t)
{
	//�͈͊O�̎��̏���
	if (t > 1.0f)t = 1.0f;
	if (t < 0.0f)t = 0.0f;

	//�P�ʃN�H�[�^�j�I��
	Quaternion rQ = IdentityQ();
	//���K���ł��Ȃ��ꍇ
	if (sRota.Magnitude() <= 0.0f || 
		eRota.Magnitude() <= 0.0f)
	{
		//��]���Ȃ��N�H�[�^�j�I����Ԃ�
		return rQ;
	}
	//�J�n�ƏI���̃N�H�[�^�j�I���𐳋K��
	Quaternion sRotaNom = sRota.NormQ();
	Quaternion eRotaNom = eRota.NormQ();
	//�J�n�ƏI���̃N�H�[�^�j�I���������Ȃ�
	if (sRotaNom == eRotaNom)
	{
		//��]���Ȃ��N�H�[�^�j�I����Ԃ�
		return rQ;
	}

	//���ς���p�x�����߂�
	float dot = sRotaNom.DotQ(eRotaNom);
	//���̎��������]
	if (dot < 0.0f)
	{
		dot *= -1.0f;
	}
	float rad = acos(dot);
	
	//���ʐ��`��Ԃ̌������
	float s1 = sin((1.0f - t) * rad) / sin(rad);
	float s2 = sin(t * rad) / sin(rad);
	rQ = (sRotaNom * s1) + (eRotaNom * s2);
	return rQ;
}



//Quaternion Quaternion::LookAt(const Vector3& targetDir, const Vector3& up)
//{
//	//�P�ʃN�H�[�^�j�I��
//	Quaternion rQ = IdentityQ();
//	assert(targetDir.Magnitude() > 0.0f && "����������܂���");
//	Vector3 forwardDir = targetDir.Normalize();
//	//��]���Ȃ��Ȃ�
//	if (forwardDir == Vector3::Forward())
//	{
//		//��]���Ȃ��N�H�[�^�j�I����Ԃ�
//		return rQ;
//	}
//
//	//���ς���p�x�����߂�
//	float dot = Vector3::Forward().Dot(targetDir);
//	//���̎��������]
//	if (dot < 0.0f)
//	{
//		dot *= -1.0f;
//	}
//	float theta = acos(dot);
//
//	//��]�̎������߂�
//	Vector3 axis = { 0.0f,1.0f,0.0f };
//	//�^���������N�H�[�^�j�I���ȊO�Ȃ�
//	if ((forwardDir * -1.0f) != Vector3::Forward())
//	{
//		//�����O�ςŋ��߂�
//		axis = Vector3::Forward().Cross(targetDir);
//	}
//	//�N�H�[�^�j�I���쐬
//	rQ = AngleAxis(theta, axis);
//	return rQ;
//}

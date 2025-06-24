#include "Vector3.h"
#include "Vector2.h"
#include "MathSub.h"
#include <cmath>
#include <cassert>

//3D�p
Vector3::Vector3() :
    x(0.0f),
    y(0.0f),
    z(0.0f)
{
}

Vector3::Vector3(float initX, float initY, float initZ) :
    x(initX),
    y(initY),
    z(initZ)
{
}

Vector3::Vector3(VECTOR vec):
    x(vec.x),
    y(vec.y),
    z(vec.z)
{
}

float Vector3::Magnitude() const
{
    //�O�����̒藝�Ōv�Z
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

float Vector3::SqMagnitude() const
{
    return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vector3 Vector3::Normalize() const
{
    if (this->Magnitude() <= 0.0f)
    {
        return *this;
    }
//    assert(this->Magnitude() > 0.0f && "���K���Ɏ��s���܂���");
    //���K��
    Vector3 ans;
    ans = (*this) / Magnitude();
    return ans;
}

float Vector3::Dot(const Vector3& right) const
{
    //����
    float ans;
    ans = this->x * right.x + this->y * right.y + this->z * right.z;
    return ans;
}

Vector3 Vector3::Cross(const Vector3& right) const
{
    //�O��
    Vector3 ans;
    ans.x = this->y * right.z - this->z * right.y;
    ans.y = this->z * right.x - this->x * right.z;
    ans.z = this->x * right.y - this->y * right.x;
    return ans;
    //a��b�̃x�N�g������镽�ʂɐ�����a����b��
    //�E�˂����񂵂��Ƃ��ɂ˂��̐i�����������傫����
    //a��b����镽�s�l�ӌ`�̖ʐςɓ�����
}

DxLib::VECTOR Vector3::ToDxLibVector() const
{
    return VGet(this->x,this->y,this->z);
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	//���`���
	if (t < 0.0f)t = 0.0f;
	if (t > 1.0f)t = 1.0f;
	Vector3 rV;
	rV = start * (1.0f - t) + end * t;
    return rV;
}

Vector2 Vector3::XZ()
{
    Vector2 rV;
    rV.x = this->x;
    rV.y = this->z;
    return rV;
}

Vector3 Vector3::operator+(const Vector3& right) const
{
    Vector3 ans;
    ans.x = this->x + right.x;
    ans.y = this->y + right.y;
    ans.z = this->z + right.z;
    return ans;
}

Vector3 Vector3::operator-(const Vector3& right) const
{
    Vector3 ans;
    ans.x = this->x - right.x;
    ans.y = this->y - right.y;
    ans.z = this->z - right.z;
    return ans;
}

Vector3 Vector3::operator*(const float& right) const
{
    Vector3 ans;
    ans.x = this->x * right;
    ans.y = this->y * right;
    ans.z = this->z * right;
    return ans;
}

Vector3 Vector3::operator/(const float& right) const
{
    Vector3 ans;
    ans.x = this->x / right;
    ans.y = this->y / right;
    ans.z = this->z / right;
    return ans;
}

Vector3 Vector3::operator=(const float& right) const
{
    Vector3 ans;
    ans.x = right;
    ans.y = right;
    ans.z = right;
    return ans;
}

Vector3 Vector3::operator+=(const Vector3& right)
{
    this->x = this->x + right.x;
    this->y = this->y + right.y;
    this->z = this->z + right.z;
    return *this;
}

Vector3 Vector3::operator-=(const Vector3& right)
{
    this->x = this->x - right.x;
    this->y = this->y - right.y;
    this->z = this->z - right.z;
    return *this;
}

Vector3 Vector3::operator*=(const float& right)
{
    this->x = this->x * right;
    this->y = this->y * right;
    this->z = this->z * right;
    return *this;
}

Vector3 Vector3::operator/=(const float& right)
{
    this->x = this->x / right;
    this->y = this->y / right;
    this->z = this->z / right;
    return *this;
}

bool Vector3::operator==(const Vector3& right) const
{
    bool isFlag = (this->x == right.x && this->y == right.y && this->z == right.z);
    return isFlag;
}

bool Vector3::operator!=(const Vector3& right) const
{
    bool isFlag = (this->x != right.x || this->y != right.y || this->z != right.z);
    return isFlag;
}

Vector3 Vector3::Zero()
{
    Vector3 zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    return zero;
}


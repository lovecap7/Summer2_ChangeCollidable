#include "Vector2.h"
#include "Vector3.h"
#include "MathSub.h"
#include <DxLib.h>
#include <cassert>

//2D�p
float Vector2::Magnitude() const
{
    //�O�����̒藝�Ōv�Z
    return sqrtf(this->x * this->x + this->y * this->y);
}

Vector2 Vector2::Normalize()const
{
    //���K�������l��Ԃ�
    Vector2 ans = *this / Magnitude();
    return ans;
}

float Vector2::Dot(const Vector2& right) const
{
    float ans;
    ans = this->x * right.x + this->y * right.y;
    return ans;
}

float Vector2::Cross(const Vector2& right) const
{
    //�O��(�X�J���[)
    float ans;
    ans = this->x * right.y - this->y * right.x;
    return ans;
    //a��b�̃x�N�g������镽�ʂɐ�����a����b��
    //�E�˂����񂵂��Ƃ��ɂ˂��̐i�����������傫����
    //a��b����镽�s�l�ӌ`�̖ʐςɓ�����
}

Vector2 Vector2::Zero() const
{
    Vector2 zaro{ 0.0f ,0.0f };
    return zaro;
}

Vector2 Vector2::operator+(const Vector2& right) const
{
    Vector2 ans;
    ans.x = this->x + right.x;
    ans.y = this->y + right.y;
    return ans;
}

Vector2 Vector2::operator-(const Vector2& right) const
{
    Vector2 ans;
    ans.x = this->x - right.x;
    ans.y = this->y - right.y;
    return ans;
}

Vector2 Vector2::operator*(const float& right) const
{
    Vector2 ans;
    ans.x = this->x * right;
    ans.y = this->y * right;
    return ans;
}

Vector2 Vector2::operator/(const float& right) const
{
    Vector2 ans;
    ans.x = this->x / right;
    ans.y = this->y / right;
    return ans;
}

Vector2 Vector2::operator+=(const Vector2& right)
{
    this->x += right.x;
    this->y += right.y;
    return *this;
}

Vector2 Vector2::operator-=(const Vector2& right)
{
    this->x -= right.x;
    this->y -= right.y;
    return *this;
}

Vector2 Vector2::operator*=(const float& right)
{
    this->x *= right;
    this->y *= right;
    return *this;
}

Vector2 Vector2::operator/=(const float& right)
{
    //0���ȊO�Ȃ�v�Z����
    assert(right != 0.0f);
    this->x /= right;
    this->y /= right;
    return *this;
}

bool Vector2::operator==(const Vector2& right) const
{
    bool isX = this->x == right.x;
    bool isY = this->y == right.y;
    return isX && isY;
}

bool Vector2::operator!=(const Vector2& right) const
{
    bool isX = this->x != right.x;
    bool isY = this->y != right.y;
    return isX || isY;
}

float Vector2::Theata(Vector2 vec1, Vector2 vec2)
{
    //�x�N�g�����Ȃ�
    if (vec1.Magnitude() <= 0.0f || vec2.Magnitude() <= 0.0f)
    {
        return 0.0f;
    }

    //���K��
    Vector2 v1 = vec1.Normalize();
    Vector2 v2 = vec2.Normalize();

    //���ς���Ԃ̊p�x���擾
    float dot = v1.Dot(v2);
    //�N�����v
    dot = MathSub::ClampFloat(dot, -1.0f, 1.0f);
    //��(�p�x)���擾
    float theta = acosf(dot);

	return theta;
}

Vector3 Vector2::XZ()
{
    Vector3 rV;
    rV.x = this->x;
    rV.y = 0.0f;
    rV.z = this->y;
    return rV;
}

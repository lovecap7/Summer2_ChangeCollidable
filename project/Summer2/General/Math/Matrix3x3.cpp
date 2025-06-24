#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
{
    IdentityMat3x3();
}

//�P�ʍs��
Matrix3x3 IdentityMat3x3()
{
    Matrix3x3 identity = {};//�v�f�����ׂ�0�ɂ���
    identity.mat[0][0] = 1.0f;
    identity.mat[1][1] = 1.0f;
    identity.mat[2][2] = 1.0f;
    return identity;
}

//���s�ړ��s��
Matrix3x3 TranslateMat3x3(float x, float y)
{
    Matrix3x3 translate = {};
    translate = IdentityMat3x3();//�P�ʍs��ɂ���
    translate.mat[0][2] = x;
    translate.mat[1][2] = y;
    return translate;
}

//��]�s��
Matrix3x3 RotateMat3x3(float angle)
{
    Matrix3x3 rotate = {};
    rotate = IdentityMat3x3();//�P�ʍs��ɂ���
    //[cos][-sin]
    //[sin][cos]
    rotate.mat[0][0] = cosf(angle);
    rotate.mat[0][1] = -sinf(angle);
    rotate.mat[1][0] = sinf(angle);
    rotate.mat[1][1] = cosf(angle);
    return rotate;
}

//�Q�̍s��̏�Z��Ԃ�
Matrix3x3 Matrix3x3::MultipleMat3x3(const Matrix3x3& left, const Matrix3x3& right)
{
    //����ɏ�Z���ʂ����Ă���
    Matrix3x3 mul = {};
    for (int k = 0;k <= 2;++k)
    {
        for (int j = 0;j <= 2;++j)
        {
            for (int i = 0;i <= 2;++i)
            {
                mul.mat[k][j] += left.mat[k][i] * right.mat[i][j];
            }
        }
    }
    //mul.mat[0][0] = 
    //    left.mat[0][0] * right.mat[0][0] + 
    //    left.mat[0][1] * right.mat[1][0] + 
    //    left.mat[0][2] * right.mat[2][0];
    //mul.mat[0][1] =
    //    left.mat[0][0] * right.mat[0][1] +
    //    left.mat[0][1] * right.mat[1][1] +
    //    left.mat[0][2] * right.mat[2][1];
    //...
    return mul;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& right)
{
    return MultipleMat3x3(*this, right);
}

//�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
Vector2 Matrix3x3::MultipleVec2(const Matrix3x3& m, const Vector2& v)
{
    Vector2 vec = {};
    vec.x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * 1.0f;
    vec.y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * 1.0f;
    return vec;
}

Vector2 Matrix3x3::operator*(const Vector2& v)
{
    return MultipleVec2(*this, v);
}


///�Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂�
Matrix3x3 RotatePositionMatrix3x3(const Vector2& center, float angle)
{
    //���S�����_�ɕ��s�ړ�������
    //���_���S�ɉ�]
    //���S�����Ƃ̍��W�ɖ߂�
    Matrix3x3 rotaMat = TranslateMat3x3(center.x, center.y) *//���̍��W
        RotateMat3x3(angle) *//��]
        TranslateMat3x3(-center.x, -center.y);//���_�ɖ߂�
    return rotaMat;
}

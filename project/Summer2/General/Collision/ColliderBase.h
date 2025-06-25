#pragma once

/// <summary>
/// �����蔻��̌`�ɂ���ď������ς��̂�
/// ���݂����ǂ̌`�̓����蔻������Ă���̂���
/// ��Ƀ`�F�b�N����ۂɎg��
/// </summary>
enum class Shape
{
	None,
	Sphere,
	Capsule,
	Polygon
};

/// <summary>
/// �����蔻��̌`�Ɋւ�����N���X
/// </summary>
class ColliderBase abstract
{
public:
	ColliderBase(Shape shape);
	virtual ~ColliderBase() {};
	//�����蔻��̌`
	Shape m_shape;
};


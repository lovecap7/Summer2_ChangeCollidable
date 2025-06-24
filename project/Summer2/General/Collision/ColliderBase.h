#pragma once

/// <summary>
/// �����蔻��̌`�ɂ���ď������ς��̂�
/// ���݂����ǂ̌`�̓����蔻������Ă���̂���
/// ��Ƀ`�F�b�N����ۂɎg��
/// </summary>
enum class Shape
{
	Sphere,
	Capsule,
	Box,
	Polygon
};

/// <summary>
/// �����蔻��̌`�Ɋւ�����N���X
/// </summary>
class ColliderBase abstract
{
public:
	ColliderBase(Shape kind);
	virtual ~ColliderBase() {};

	/// <summary>
	/// �����蔻��̌`���擾
	/// </summary>
	/// <returns>�����蔻��̌`</returns>
	Shape GetShape()const { return m_kind; }
private:
	//�����蔻��̌`
	Shape m_kind;
};


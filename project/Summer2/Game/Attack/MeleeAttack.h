#pragma once
#include "AttackBase.h"
class MeleeAttack :
    public AttackBase, public std::enable_shared_from_this<MeleeAttack>
{
public:
	/// <summary>
	/// �ߐڍU��
	/// </summary>
	/// <param name="coll">�Փˊm�F�̂��߂̏��</param>
	/// <param name="damage">�_���[�W</param>
	/// <param name="keepFrame">����</param>
	/// <param name="knockBack">�m�b�N�o�b�N�̑傫��</param>
	/// <param name="owner">������</param>
	MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner);
	~MeleeAttack() override;
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;
	//����������
	void OnHit(std::shared_ptr<Actor> actor) override;
};


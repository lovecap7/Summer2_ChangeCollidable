#pragma once
#include "AttackBase.h"
#include "../../General/Math/Vector3.h"
class BlastAttack :
    public AttackBase, public std::enable_shared_from_this<BlastAttack>
{
public:
	/// <summary>
	/// �͈͍U��
	/// </summary>
	/// <param name="coll">�Փˊm�F�̂��߂̏��</param>
	/// <param name="damage">�_���[�W</param>
	/// <param name="keepFrame">����</param>
	/// <param name="knockBack">�m�b�N�o�b�N�̑傫��</param>
	/// <param name="owner">������</param>
	BlastAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner);
	~BlastAttack() override;
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;
	//����������
	void OnHit(std::shared_ptr<Actor> actor) override;
	//������΂�����
	Vector3 GetKnockBackVec(Vector3 other) override;
};

#include "PlayerStateChargeAttackBase.h"
#include "../../Attack/Strike.h"
#include "../../../../General/Model.h"
#include "../../ActorManager.h"

namespace
{
	//�����̍��{�Ƒ���̃C���f�b�N�X
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
}

PlayerStateChargeAttackBase::PlayerStateChargeAttackBase(std::weak_ptr<Actor> player):
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
}

PlayerStateChargeAttackBase::~PlayerStateChargeAttackBase()
{
}

void PlayerStateChargeAttackBase::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//�U�����쐬
	auto attack = std::make_shared<Strike>(m_owner);
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, Battle::AttackWeight::Middle);
	//�Q��
	m_attack = attack;
	//�U��������
	actorManager.lock()->AddNextActor(attack);
}

void PlayerStateChargeAttackBase::UpdateStrikeAttackPos(std::weak_ptr<Strike> attack)
{
	//�����̏�Ԃ��X�V������U�����X�V�����
	auto model = m_owner.lock()->GetModel();
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//����
	//���W���Z�b�g
	attack.lock()->SetStartPos(root);
	attack.lock()->SetEndPos(toe);
}

#include "PlayerStateAttackNormalBase.h"
#include "../../Attack/Slash.h"
#include "../../../../General/Model.h"
#include "../../ActorManager.h"
namespace
{
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒����Ɣ��a
	constexpr float kSwordHeight = 150.0f;
}

PlayerStateAttackNormalBase::PlayerStateAttackNormalBase(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
}

PlayerStateAttackNormalBase::~PlayerStateAttackNormalBase()
{
}
void PlayerStateAttackNormalBase::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//�U�����쐬
	auto attack = std::make_shared<Slash>(m_owner);
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, Battle::AttackWeight::Middle);
	//�Q��
	m_attack = attack;
	//�U��������
	actorManager.lock()->AddNextActor(attack);
}
void PlayerStateAttackNormalBase::UpdateSlashAttackPos(std::weak_ptr<Slash> attack)
{
	auto model = m_owner.lock()->GetModel();
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	//���W���Z�b�g
	attack.lock()->SetStartPos(ringFinger);
	attack.lock()->SetEndPos(swordDir);
}

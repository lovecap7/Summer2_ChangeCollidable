#include "PlayerStateAttackNormalBase.h"
#include "../../Attack/Slash.h"
#include "../../../../General/Model.h"
#include "../../ActorManager.h"
#include "../../../../General/Rigidbody.h"
#include "Player.h"
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
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}
void PlayerStateAttackNormalBase::UpdateAttackPos()
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
	m_attack.lock()->SetStartPos(ringFinger);
	m_attack.lock()->SetEndPos(swordDir);
}
void PlayerStateAttackNormalBase::AttackMove(float speed)
{
	//�^�[�Q�b�g�����G���Ă�Ȃ�(�^�[�Q�b�g�̂ق�������)
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//����
	Vector3 dir = coll->GetStickVec().XZ();
	//�^�[�Q�b�g�������ł�����
	if (targetData.isHitTarget)
	{
		dir = targetData.targetDirXZ;
	}
	//�����̍X�V
	coll->GetModel()->SetDir(dir.XZ());
	//�����Ă�����Ɉړ�
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * speed);
}

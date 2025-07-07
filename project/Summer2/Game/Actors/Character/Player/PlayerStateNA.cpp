#include "PlayerStateNA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../Attack/Slash.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒���
	constexpr float kSwordHeight = 150.0f;
	//�L�����Z��
	constexpr int kAttackCancelFrame = 20;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A���U���̍ő吔
	constexpr int kMaxComboNum = 3;
}

PlayerStateNA::PlayerStateNA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_comboNum(1),
	m_attackCountFrame(0)
{
	//�U���f�[�^�̏�����
	InitAttackData(actorManager);
}

PlayerStateNA::~PlayerStateNA()
{
}
void PlayerStateNA::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateNA::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���������Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//���S�������{�X���|���ĂȂ��ꍇ
	if (coll->GetHitPoints().lock()->IsDead() && !actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	auto model = coll->GetModel();
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//�U���쐬
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame, 
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
			return;
		}
		//�܂��U���ł���Ȃ�
		if (input.IsTrigger("X") && m_comboNum < kMaxComboNum)
		{
			//���̍U��
			++m_comboNum;
			//������
			InitAttackData(actorManager);
			m_attackCountFrame = 0;
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�`���[�W
			ChangeState(std::make_shared<PlayerStateCharge>(m_owner));
			return;
		}
	}

	//�U���̈ʒu�X�V
	UpdateAttackPos();
	//�ړ��t���[�����͑O�ɐi��
	if (m_attackCountFrame <= m_attackData.moveFrame)
	{
		AttackMove(m_attackData.moveSpeed);
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
void PlayerStateNA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}
void PlayerStateNA::UpdateAttackPos()
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
	//�G�t�F�N�g�̈ʒu�X�V
	if (!m_eff.expired())
	{
		m_eff.lock()->SetPos(swordDir);
	}
	if (!m_attack.expired())
	{
		//���W���Z�b�g
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordDir);
	}
}
void PlayerStateNA::AttackMove(float speed)
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

void PlayerStateNA::InitAttackData(const std::weak_ptr<ActorManager> actorManager)
{
	//�a���G�t�F�N�g
	m_eff = EffekseerManager::GetInstance().CreateEffect("SwordTest", m_owner.lock()->GetPos());
	std::string attackName;
	switch (m_comboNum)
	{
	case 1:
		attackName = kNA1Name;
		break;
	case 2:
		attackName = kNA2Name;
		break;
	case kMaxComboNum:
		attackName = kNA3Name;
		break;
	default:
		break;
	}
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, attackName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�ʏ�U��1
	coll->SetCollState(CollisionState::Move);
	//�U��1
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//���Z�Q�[�W�̗\��
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
}
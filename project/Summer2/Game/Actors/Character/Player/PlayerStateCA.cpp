#include "PlayerStateCA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "UltGage.h"
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
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../Attack/Slash.h"

namespace
{
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒���
	constexpr float kSwordHeight = 150.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�G�t�F�N�g�̍폜��x�点��
	constexpr int kDeleteEffectDelayFrame = 20;
}

PlayerStateCA::PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager, AttackData data)	:
	PlayerStateBase(player),
	m_attackCountFrame(0),
	m_attackData(data)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�`���[�W�U��
	auto model = coll->GetModel();
	model->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//���Z�Q�[�W�̗\��
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
}

PlayerStateCA::~PlayerStateCA()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
}
void PlayerStateCA::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//�{�X�����S�ɏ��ł����Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ�܂��̓A�j���[�V�������I��������
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead() ||
		model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//���S�������{�X���|���ĂȂ��ꍇ
	if (coll->GetHitPoints().lock()->IsDead())
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
	++m_attackCountFrame;
	//�����t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//�U������
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame,
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	//�������U���񐔂Ŋ������l�ɒB�����Ƃ�
	if (m_attackCountFrame % (m_attackData.keepFrame / m_attackData.attackNum) == 0)
	{
		m_attack.lock()->ResetHitId();
	}
	//�U���̈ʒu�X�V
	if (!m_attack.expired())
	{
		UpdateAttackPos();
	}
	//���͂�����Ƃ�
	auto& input = Input::GetInstance();
	if (input.GetStickInfo().IsLeftStickInput() &&
		m_attackCountFrame <= m_attackData.moveFrame)
	{
		//�ړ��t���[�����͑O�ɐi��
		AttackMove(m_attackData.moveSpeed);
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PlayerStateCA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void PlayerStateCA::UpdateAttackPos()
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
	if (!m_attack.expired())
	{
		//���W���Z�b�g
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordDir);
	}
}

void PlayerStateCA::AttackMove(float speed)
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

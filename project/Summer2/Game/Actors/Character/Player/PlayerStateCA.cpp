#include "PlayerStateCA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
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
#include "../../Attack/Strike.h"

namespace
{
	//�����̍��{�Ƒ���̃C���f�b�N�X
	constexpr int kRootIndex = 60;
	constexpr int kToeIndex = 64;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�G�t�F�N�g�̍폜��x�点��
	constexpr int kDeleteEffectDelayFrame = 20;
}

PlayerStateCA::PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager, int chargeFrame)	:
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	//�`���[�W�t���[���������t���[�����傫�������r
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
	if (chargeFrame > m_attackData.keepFrame)
	{
		//2�i�K��
		m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
		if (chargeFrame > m_attackData.keepFrame)
		{
			//3�i�K��
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA3Name);
		}
	}
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�`���[�W�U��
	auto model = coll->GetModel();
	model->SetAnim(m_attackData.anim.c_str(), true, m_attackData.animSpeed);
	model->SetFixedLoopFrame(m_attackData.keepFrame);//�w�胋�[�v
	//���Z�Q�[�W�̗\��
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);

	//�L�b�N�G�t�F�N�g
	m_eff = EffekseerManager::GetInstance().CreateTrackActorEffect("CATornade", m_owner.lock());
}

PlayerStateCA::~PlayerStateCA()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	//�G�t�F�N�g�𐔃t���[����폜
	m_eff.lock()->SpecificFrame(kDeleteEffectDelayFrame);
}
void PlayerStateCA::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	++m_attackCountFrame;
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//�U������
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame,
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	auto model = coll->GetModel();
	//�A�j���[�V�������I��������
	if (model->IsFinishFixedLoop())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�A�j���[�V������������邽�тɍU������̃��Z�b�g
	if (model->IsFinishAnim())
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//�U���̈ʒu�X�V
	if (!m_attack.expired())
	{
		UpdateAttackPos();
	}
	auto& input = Input::GetInstance();
	//���͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�ړ�
		coll->GetRb()->SetMoveVec(GetForwardVec(camera) * m_attackData.moveSpeed);
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PlayerStateCA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Strike>(actorManager.lock()->CreateAttack(AttackType::Strike, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}

void PlayerStateCA::UpdateAttackPos()
{
	//�����̏�Ԃ��X�V������U�����X�V�����
	auto model = m_owner.lock()->GetModel();
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//����
	//���W���Z�b�g
	m_attack.lock()->SetStartPos(root);
	m_attack.lock()->SetEndPos(toe);
}

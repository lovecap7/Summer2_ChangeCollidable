#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../Attack/AreaOfEffectAttack.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//Ult�̃_���[�W�Ǝ����t���[��
	constexpr int kUltDamege = 150.0f;
	constexpr int kUltKeepFrame = 180;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 2.0f;
	//�U����������Z�b�g����p�x
	constexpr int kUltResetFrame = 10;
	//�A�j���[�V����
	const char* kAnim = "Player|Ult2";
	//�A�j���[�V�����̑��x�̏����l
	constexpr float kAnimSpeed = 0.7f;
	//�A�j���[�V�����̑��x�̕ω���
	constexpr float kAddAnimSpeed = 0.2f;
	//�A�j���[�V�����̍ō����x
	constexpr float kMaxAnimSpeed = 2.0f;
	//����̍��W�Ɠ����蔻��̏��
	constexpr float kAttackDistance = 200.0f;//�v���C���[�̑O���ɍU��������o���ۂ̋���
	constexpr float kAttackRadius = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateUltimate::PlayerStateUltimate(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_animCountFrame(0),
	m_animSpeed(kAnimSpeed)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�K�E�Z
	coll->SetCollState(CollisionState::Normal);
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, m_animSpeed);
	model->SetFixedLoopFrame(kUltKeepFrame);//�w�胋�[�v
	//�����̍X�V
	Vector2 dir = coll->GetStickVec();
	model->SetDir(dir);
	//�Q�[�W��0��
	coll->GetUltGage().lock()->ResetUltGage();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
}


PlayerStateUltimate::~PlayerStateUltimate()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}

void PlayerStateUltimate::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateUltimate::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	if (m_animCountFrame == 1)
	{
		//�U������
		CreateAttack(actorManager);
	}

	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//���������Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�A�j���[�V�������I��������
	if (model->IsFinishFixedLoop())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�U����������Z�b�g
	if (m_animCountFrame % kUltResetFrame == 0)
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//�A�j���[�V������������邽�тɍĐ����x���グ��
	if (model->IsFinishAnim())
	{
		//�A�j���[�V�����̑��x�𑬂����Ă���
		if (m_animSpeed < kMaxAnimSpeed)
		{
			m_animSpeed += kAddAnimSpeed;
			model->SetAnimSpeed(m_animSpeed);
		}
	}
	//�U���̈ʒu�X�V
	if (!m_attack.expired())UpdateAttackPos();
	//��������������
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void PlayerStateUltimate::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<AreaOfEffectAttack>(actorManager.lock()->CreateAttack(AttackType::AreaOfEffect, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	attack->SetRadius(kAttackRadius);
	attack->AttackSetting(kUltDamege, kUltKeepFrame, kKnockBackPower,Battle::AttackWeight::Heaviest);
}

void PlayerStateUltimate::UpdateAttackPos()
{
	auto coll = m_owner.lock();
	auto model = coll->GetModel();
	//�v���C���[�̑O������ɏo��
	Vector3 attackPos = coll->GetPos() + model->GetDir().Normalize() * kAttackDistance;
	m_attack.lock()->SetPos(attackPos);
}

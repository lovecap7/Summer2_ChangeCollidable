#include "PlayerStateCA3.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
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
#include "../../../../Game/Camera/Camera.h"
#include "../../Attack/Strike.h"

namespace
{
	//�`���[�W�U���̒i�K�ʃ_���[�W
	constexpr int kCA3AnimDamage = 160.0f;
	//�`���[�W�U���̎���
	constexpr float kCA3KeepFrame = 60.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 3.0f;
	//�U���̔���
	constexpr int kStartFrame = 1;
	//�A�j���[�V����
	const char* kAnim = "Player|CA";//�`���[�W�U��
	//�`���[�W�U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCA3AnimSpeed = 3.0f;
	//�����̔��a
	constexpr float kAttackRadius = 20.0f;
	//�ړ����x
	constexpr float kAttackMoveSpeed = 10.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���Z�Q�[�W��
	constexpr int kAddUltGage = 3;
}

PlayerStateCA3::PlayerStateCA3(std::weak_ptr<Actor> player) :
	PlayerStateChargeAttackBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//�`���[�W�U��1
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, kCA3AnimSpeed);
	model->SetFixedLoopFrame(kCA3KeepFrame);//�w�胋�[�v
}


PlayerStateCA3::~PlayerStateCA3()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
}
void PlayerStateCA3::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA3::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���S�����Ȃ�
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
	auto& input = Input::GetInstance();
	++m_attackCountFrame;
	if (m_attackCountFrame == kStartFrame)
	{
		//�U������
		CreateAttack(kAttackRadius, kCA3AnimDamage, kCA3KeepFrame, kKnockBackPower, Battle::AttackWeight::Middle, actorManager);
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
	//���͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�ړ�
		coll->GetRb()->SetMoveVec(GetForwardVec(camera) * kAttackMoveSpeed);
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

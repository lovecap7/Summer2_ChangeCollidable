#include "PlayerStateCA2.h"
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
	constexpr int kCA2AnimDamage = 150.0f;
	//�`���[�W�U���̎���
	constexpr float kCA2KeepFrame = 60.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 2.0f;
	//�U���̔���
	constexpr int kStartFrame = 1;
	//�A�j���[�V����
	const char* kAnim = "Player|CA";//�`���[�W�U��
	//�`���[�W�U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCA2AnimSpeed = 2.5f;
	//�����̔��a
	constexpr float kAttackRadius = 20.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���Z�Q�[�W��
	constexpr int kAddUltGage = 2;
}

PlayerStateCA2::PlayerStateCA2(std::weak_ptr<Actor> player) :
	PlayerStateChargeAttackBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�`���[�W�U��1
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, kCA2AnimSpeed);
	model->SetFixedLoopFrame(kCA2KeepFrame);//�w�胋�[�v
}

PlayerStateCA2::~PlayerStateCA2()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage()->SetPendingUltGage(0);
}
void PlayerStateCA2::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA2::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���S�����Ȃ�
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	++m_attackCountFrame;
	if (m_attackCountFrame == kStartFrame)
	{
		//�U������
		CreateAttack(kAttackRadius, kCA2AnimDamage, kCA2KeepFrame, kKnockBackPower, Battle::AttackWeight::Middle, actorManager);
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
		UpdateStrikeAttackPos(m_attack);
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}


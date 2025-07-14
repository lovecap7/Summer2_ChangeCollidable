#include "SmallDragonStateAttack.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../Attack/Bullet.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�����ʒu�̍���
	constexpr float kBulletCreatePosY = 100.0f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 150;//2.5�b���炢�̊��o�ōU��
}

SmallDragonStateAttack::SmallDragonStateAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	SmallDragonStateBase(owner),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kAttackName);
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
}

SmallDragonStateAttack::~SmallDragonStateAttack()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void SmallDragonStateAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		return;
	}
	
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void SmallDragonStateAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//�����ʒu
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//�e�̐i�s�����ƃX�s�[�h
	attack->SetVec(coll->GetModel()->GetDir() * data.moveSpeed);
	//�G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("BulletEff", std::dynamic_pointer_cast<Actor>(attack));
}
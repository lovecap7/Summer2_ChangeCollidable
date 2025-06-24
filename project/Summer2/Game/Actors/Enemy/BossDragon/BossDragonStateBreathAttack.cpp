#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/BulletAttack.h"
#include "../../../Attack/AttackManager.h"
#include "../../ActorManager.h"
#include "../../../../General/Math/Quaternion.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�e�̔��a�̑傫��
	constexpr float kBulletRadius = 40.0f;
	//�e�̃_���[�W
	constexpr int kBulletDamage = 100;
	//�e�̎����t���[��
	constexpr int kBulletKeepFrame = 180;
	//�e�̔����t���[��
	constexpr int kBulletFireFrame = 25;
	//�e�̃X�s�[�h
	constexpr float kBulletSpeed = 7.0f;
	//�e�̔��ˊp�x
	constexpr float kBulletAngle = 30.0f / 180.0f * DX_PI_F;
	//�e�̐����ʒu��Y���W
	constexpr float kBulletCreatePosY = 150.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 12.0f;

	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Headbutt";
	//�A�j���[�V�����̑��x
	constexpr float kAnimSpeed = 0.3f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 100;
}

BossDragonStateBreathAttack::BossDragonStateBreathAttack(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//�ʏ�U��
	m_owner->GetCollidable()->SetState(State::None);
	//�U��
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//�v���C���[������
	LookPlayer();
}

BossDragonStateBreathAttack::~BossDragonStateBreathAttack()
{
	//�U���̃N�[���^�C��
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}
void BossDragonStateBreathAttack::Init()
{
	// ���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateBreathAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}

	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kBulletFireFrame)
	{
		//�U������̏���
		CreateAttack();
		//�U��������
		AppearAttack(m_bullet1, attackManager);
		AppearAttack(m_bullet2, attackManager);
		AppearAttack(m_bullet3, attackManager);
	}
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
	}

	//����
	SpeedDown();
}

void BossDragonStateBreathAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//�����ʒu
	Vector3 bulletPos = m_owner->GetCollidable()->GetRb()->GetPos();
	bulletPos.y += kBulletCreatePosY;
	//�e�̍쐬
	CreateBullet(bulletPos, model,m_bullet1);
	CreateBullet(bulletPos, model,m_bullet2);
	CreateBullet(bulletPos, model,m_bullet3);
	//�e�̐i�s�����ƃX�s�[�h
	Vector3 bulletDir = model->GetDir();
	m_bullet1->SetDirAndSpeed(bulletDir, kBulletSpeed);//�^���������
	bulletDir = Quaternion::AngleAxis(kBulletAngle,Vector3::Up()) *model->GetDir();
	m_bullet2->SetDirAndSpeed(bulletDir, kBulletSpeed);//�΂߂ɔ��
	bulletDir = Quaternion::AngleAxis(-kBulletAngle, Vector3::Up()) * model->GetDir();
	m_bullet3->SetDirAndSpeed(bulletDir, kBulletSpeed);//�΂߂ɔ��
}

void BossDragonStateBreathAttack::CreateBullet(Vector3& bulletPos, std::shared_ptr<Model>& model, std::shared_ptr<BulletAttack>& bullet)
{
	//�e�̓����蔻��쐬
	auto coll = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kBulletRadius),
		std::make_shared<Rigidbody>(bulletPos));
	//�e�̍��W�Ɠ����蔻����U���ɕR�}����
	bullet = std::make_shared<BulletAttack>(coll, kBulletDamage, kBulletKeepFrame, kKnockBackPower, m_owner);
}

void BossDragonStateBreathAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

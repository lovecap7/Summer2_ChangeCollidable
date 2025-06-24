#include "PlayerStateCA3.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/HurtPoint.h"
#include "../ActorManager.h"
namespace
{
	//�`���[�W�U���̒i�K�ʃ_���[�W
	constexpr int kCA3AnimDamage = 160.0f;
	//�`���[�W�U���̎���
	constexpr float kCA3KeepFrame = 60.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 3.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|CA";//�`���[�W�U��
	//�`���[�W�U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCA3AnimSpeed = 3.0f;
	//�����̍��{�Ƒ���̃C���f�b�N�X
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//�����̔��a
	constexpr float kLeftLegRadius = 20.0f;
	//�ړ����x
	constexpr float kAttackMoveSpeed = 10.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���Z�Q�[�W��
	constexpr int kAddUltGage = 3;
}

PlayerStateCA3::PlayerStateCA3(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager) :
	PlayerStateBase(player)
{
	m_player->GetCollidable()->SetState(State::None);
	//�`���[�W�U��3
	auto model = m_player->GetModel();
	model->SetAnim(kAnim, true, kCA3AnimSpeed);
	model->SetFixedLoopFrame(kCA3KeepFrame);//�w�胋�[�v
	//�U������̏���
	CreateAttack();
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	attackManager->Entry(m_attackC);
	//���Z�Q�[�W�̗\��
	m_player->GetUltGage()->SetPendingUltGage(kAddUltGage);
}

PlayerStateCA3::~PlayerStateCA3()
{
	//���Z�Q�[�W�̗\��
	m_player->GetUltGage()->SetPendingUltGage(0);
}
void PlayerStateCA3::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA3::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//���S
	if (m_player->GetHurtPoint()->IsDead())
	{
		//�폜
		DeleteAttack(attackManager);
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//�U�����󂯂���
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//�폜
		DeleteAttack(attackManager);
		//������
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	//�Q�[�W������Ƃ��g����
	if (input.IsTrigger("RB") && m_player->GetUltGage()->IsMaxUlt())
	{
		//�폜
		DeleteAttack(attackManager);
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player, actorManager));
		return;
	}
	auto model = m_player->GetModel();
	//�A�j���[�V�������I��������
	if (model->IsFinishFixedLoop())
	{
		//�폜
		DeleteAttack(attackManager);
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�A�j���[�V������������邽�тɍU������̃��Z�b�g
	if (model->IsFinishAnim())
	{
		//������
		m_attackC->Init();
	}
	//���͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�ړ�
		m_player->GetCollidable()->GetRb()->SetMoveVec(GetForwardVec(input, camera) * kAttackMoveSpeed);
	}
	else
	{
		//��������������
		SpeedDown();
	}
	//�U���̈ʒu�X�V
	UpdateAttack();
}

void PlayerStateCA3::CreateAttack()
{
	auto model = m_player->GetModel();
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//����
	m_leftLeg = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(toe, kLeftLegRadius),
		std::make_shared<Rigidbody>(root));

	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attackC = std::make_shared<MeleeAttack>(m_leftLeg, kCA3AnimDamage, kCA3KeepFrame, kKnockBackPower, m_player);
}

void PlayerStateCA3::UpdateAttack()
{
	//�����̏�Ԃ��X�V������U�����X�V�����
	auto model = m_player->GetModel();
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//����
	//���W�X�V
	m_leftLeg->GetRb()->SetPos(root);
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->SetEndPos(toe);
}

void PlayerStateCA3::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateCA3::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//�U�����������
	m_attackC->Delete();
	attackManager->Exit(m_attackC);
}
#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/SphereCollider.h"
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
	//Ult�̃_���[�W�Ǝ����t���[��
	constexpr int kUltDamege = 30.0f;
	constexpr int kUltKeepFrame = 180;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 2.0f;
	//�U����������Z�b�g����p�x
	constexpr int kUltInitFrame = 10;
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

PlayerStateUltimate::PlayerStateUltimate(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_animCountFrame(0),
	m_animSpeed(kAnimSpeed)
{
	//�K�E�Z
	m_player->GetCollidable()->SetState(State::None);
	//�`���[�W�U��1
	auto model = m_player->GetModel();
	model->SetAnim(kAnim, true, m_animSpeed);
	model->SetFixedLoopFrame(kUltKeepFrame);//�w�胋�[�v
	//�U������̏���
	CreateAttack();
	//�U���}�l�[�W���[�ɓo�^
	auto attackManager = actorManager->GetAttackManager();
	attackManager->Entry(m_attackUlt);
	//�����̍X�V
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
	//���G��
	m_player->GetHurtPoint()->SetIsNoDamege(true);
	//�Q�[�W���Z�b�g
	m_player->GetUltGage()->ResetUltGage();
}


PlayerStateUltimate::~PlayerStateUltimate()
{
	//���G����
	m_player->GetHurtPoint()->SetIsNoDamege(false);
}

void PlayerStateUltimate::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateUltimate::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	auto model = m_player->GetModel();
	//�U���}�l�[�W���[
	auto attackManager = actorManager->GetAttackManager();
	//�A�j���[�V�������I��������
	if (model->IsFinishFixedLoop())
	{
		//�폜
		DeleteAttack(attackManager);
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�U����������Z�b�g
	if (m_animCountFrame % kUltInitFrame == 0)
	{
		//������
		m_attackUlt->Init();
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
	//��������������
	SpeedDown();
	//�U���̈ʒu�X�V
	UpdateAttack();
}

void PlayerStateUltimate::CreateAttack()
{
	auto model = m_player->GetModel();
	//�v���C���[�̑O������ɏo��
	Vector3 attackPos = m_player->GetCollidable()->GetRb()->GetPos() + model->GetDir().Normalize() * kAttackDistance;
	//�U���̈ʒu���o��
	auto attackColl = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kAttackRadius),
		std::make_shared<Rigidbody>(attackPos));
	//�U���̍��W�Ɠ����蔻����U���ɕR�}����
	m_attackUlt = std::make_shared<MeleeAttack>(attackColl, kUltDamege, kUltKeepFrame, kKnockBackPower,m_player);
	//�������ō���
	m_attackUlt->SetAttackPower(Battle::AttackWeight::Heaviest);
}

void PlayerStateUltimate::UpdateAttack()
{
	auto model = m_player->GetModel();
	//�v���C���[�̑O������ɏo��
	Vector3 attackPos = m_player->GetCollidable()->GetRb()->GetPos() + model->GetDir().Normalize() * kAttackDistance;
	m_attackUlt->GetCollidable()->GetRb()->SetPos(attackPos);
}

void PlayerStateUltimate::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateUltimate::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//�U�����������
	m_attackUlt->Delete();
	attackManager->Exit(m_attackUlt);
}

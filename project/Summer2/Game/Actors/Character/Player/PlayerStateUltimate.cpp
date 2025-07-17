#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
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
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../Attack/Slash.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒���
	constexpr float kSwordHeight = 1000.0f;
	//�U����������Z�b�g����p�x
	constexpr int kUltResetFrame = 10;
	//�A�j���[�V�����̑��x�̕ω���
	constexpr float kAddAnimSpeed = 0.2f;
	//�A�j���[�V�����̍ō����x
	constexpr float kMaxAnimSpeed = 2.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateUltimate::PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_animCountFrame(0)
{
	//�`���[�W�t���[���������t���[�����傫�������r
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kULTName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�K�E�Z
	coll->SetCollState(CollisionState::Normal);
	auto model = coll->GetModel();
	m_animSpeed = m_attackData.animSpeed;
	model->SetAnim(m_attackData.anim.c_str(), false, m_animSpeed);
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
	//�U�������t���[��
	if (m_animCountFrame == m_attackData.startFrame)
	{
		//�U���쐬
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame,
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	//�U����������Z�b�g
	if (m_animCountFrame % kUltResetFrame == 0)
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//��������������
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void PlayerStateUltimate::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//�쐬�ƎQ��
	auto attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//�U�����쐬
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);

	//�U���̈ʒu
	Vector3 startPos = owner->GetPos();
	Vector3 endPos = owner->GetPos() + (owner->GetModel()->GetDir() * kSwordHeight);
	//���W���Z�b�g
	attack->SetStartPos(startPos);
	attack->SetEndPos(endPos);
	m_attack = attack;
	//�G�t�F�N�g�̈ʒu
	//�K�E�G�t�F�N�g
	auto eff = EffekseerManager::GetInstance().CreateEffect("UltLaserEff", m_owner.lock()->GetPos());
	eff.lock()->SetPos(startPos);
	eff.lock()->LookAt(owner->GetModel()->GetDir());
}


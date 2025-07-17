#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�A�j���[�V����
	const char* kAnim = "Player|Charge";//�`���[�W
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�����̃C���f�b�N�X
	constexpr int kLeftLegIndex = 64;
	//�d��
	constexpr int kStopFrame = 30;
}

PlayerStateCharge::PlayerStateCharge(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_chargeFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//������
	coll->SetCollState(CollisionState::Normal);
	//�`���[�W
	coll->GetModel()->SetAnim(kAnim, true);
	//�`���[�W�G�t�F�N�g
	m_chargeEff = EffekseerManager::GetInstance().CreateEffect(std::string("ChargeEff"), coll->GetPos());
	m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel1Eff"), m_owner);
	//�ǂ̃��x���܂ŗ��߂Ă��邩���݂ăG�t�F�N�g��ς��鏀��
	//����̃��x���̎����t���[���𒴂����烌�x�����オ��
	auto level2 = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
	auto level3 = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
	m_chargeLevel2Frame = level2.keepFrame;
	m_chargeLevel3Frame = level3.keepFrame;
	//1�i�K��
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
}

PlayerStateCharge::~PlayerStateCharge()
{
	m_chargeEff.lock()->Delete();
	//�G�t�F�N�g�𐔃t���[����폜
	m_levelEff.lock()->Delete();
}
void PlayerStateCharge::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCharge::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�{�X�����S�ɏ��ł����Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead())
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
	//����{�^������������
	if (input.IsTrigger("RB"))
	{
		//���
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	//�����̍X�V
	Vector2 dir = coll->GetStickVec();
	coll->GetModel()->SetDir(dir);
	//���߂Ă鎞
	if (input.IsPress("Y") || m_chargeFrame < kStopFrame)
	{
		//�^���U���`���[�W
		++m_chargeFrame;
		//�`���[�W�G�t�F�N�g
		m_chargeEff.lock()->SetPos(coll->GetPos());
		//2�i�K��
		if (m_chargeFrame == m_chargeLevel2Frame)
		{
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
			//�i�K���ƂɃG�t�F�N�g��ύX
			m_levelEff.lock()->Delete();
			m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel2Eff"), m_owner);
		}
		//3�i�K��
		else if(m_chargeFrame == m_chargeLevel3Frame)
		{
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA3Name);
			//�i�K���ƂɃG�t�F�N�g��ύX
			m_levelEff.lock()->Delete();
			m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel3Eff"), m_owner);
		}
	}
	//�{�^���𗣂�
	else
	{
		ChangeState(std::make_shared<PlayerStateCA>(m_owner, actorManager, m_attackData));
		return;
	}
}

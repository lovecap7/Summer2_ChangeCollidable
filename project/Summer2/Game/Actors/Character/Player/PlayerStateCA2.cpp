#include "PlayerStateCA2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�`���[�W�U���̒i�K�ʃ_���[�W
	constexpr int kCA2AnimDamage = 150.0f;
	//�`���[�W�U���̎���
	constexpr float kCA2KeepFrame = 60.0f;
	//�m�b�N�o�b�N�̑傫��
	constexpr float kKnockBackPower = 2.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|CA";//�`���[�W�U��
	//�`���[�W�U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCA2AnimSpeed = 2.5f;
	//�����̍��{�Ƒ���̃C���f�b�N�X
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//�����̔��a
	constexpr float kLeftLegRadius = 20.0f;
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
}
void PlayerStateCA2::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA2::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
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

	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);

}


#include "AttackUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/AttackPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//�_���[�W�{��
	constexpr float kDamageRate = 1.5f;
	//�����t���[��
	constexpr int kAttackUpKeepFrame = 60 * 15;//15�b
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 80.0f;
	//��]��
	constexpr float kRotaAngle = 1.0f;
	//�ŏ��̓�����Ȃ��t���[��
	constexpr int kNoHitFrame = 30;
}

AttackUp::AttackUp(int modelHandle, Vector3 pos) :
	ItemBase(Shape::Sphere)
{
	m_noHitFrame = kNoHitFrame;
	//���W
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	m_rb->m_pos = firstPos;
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->m_radius = kCollRadius;
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�͂�^����
	m_rb->SetVecY(kJumpPower);
}

AttackUp::~AttackUp()
{
}

void AttackUp::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void AttackUp::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�ړ��ʂ�������
	m_rb->SetMoveVec(Vector3::Zero());
	//���
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//�����蔻�������
		m_isThrough = false;
	}
	m_model->Update();
}

void AttackUp::OnCollide(const std::shared_ptr<Collidable> other)
{
	//���Ńt���O�������Ă�Ȃ烊�^�[��
	if (m_isDelete)return;
	//�v���C���[�ɓ����������̏���
	if (other->GetGameTag() == GameTag::Player)
	{
		//�U���A�b�v�o�t
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetAttackPoints().lock()->AttackBuff(kDamageRate, Battle::AttackWeight::Heavy, kAttackUpKeepFrame);
		//�p���[�A�b�v�G�t�F�N�g
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetAttackUpEff", player);
		auto eff = EffekseerManager::GetInstance().CreateTrackActorEffect("AttackUpEff", player);
		eff.lock()->SpecificFrame(kAttackUpKeepFrame);
		//�폜
		m_isDelete = true;
	}
}

void AttackUp::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawSphere3D(
		GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->m_radius,
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void AttackUp::Complete()
{
	//���̍��W��
	m_rb->m_pos = m_rb->GetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void AttackUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kAttackUp);
}

void AttackUp::End()
{
	Collidable::End();
	m_model->End();
}

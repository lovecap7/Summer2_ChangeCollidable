#include "DefenseUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/HitPoints.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//�_���[�W�J�b�g
	constexpr float kDamageCutRate = 0.5f;
	//�����t���[��
	constexpr int kDamageCutKeepFrame = 60 * 15;//15�b
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 50.0f;
	//��]��
	constexpr float kRotaAngle = 1.0f;
	//�ŏ��̓�����Ȃ��t���[��
	constexpr int kNoHitFrame = 30;
}

DefenseUp::DefenseUp(int modelHandle, Vector3 pos) :
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

DefenseUp::~DefenseUp()
{
}

void DefenseUp::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void DefenseUp::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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

void DefenseUp::OnCollide(const std::shared_ptr<Collidable> other)
{
	//���Ńt���O�������Ă�Ȃ烊�^�[��
	if (m_isDelete)return;
	//�v���C���[�ɓ����������̏���
	if (other->GetGameTag() == GameTag::Player)
	{
		//�_���[�W�J�b�g�ƃA�[�}�[����
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetHitPoints().lock()->DefenseBuff(Battle::Armor::Heavy, kDamageCutRate, kDamageCutKeepFrame);
		//�폜
		m_isDelete = true;
	}
}

void DefenseUp::Draw() const
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

void DefenseUp::Complete()
{
	//���̍��W��
	m_rb->m_pos = m_rb->GetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void DefenseUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kDefenseUp);
}

void DefenseUp::End()
{
	Collidable::End();
	m_model->End();
}

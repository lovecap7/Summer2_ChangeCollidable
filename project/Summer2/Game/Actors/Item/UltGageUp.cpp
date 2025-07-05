#include "UltGageUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//�Q�[�W�㏸��
	constexpr float kGageValue = 20.0f;
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 50.0f;
	//��]��
	constexpr float kRotaAngle = 2.0f;
	//�ŏ��̓�����Ȃ��t���[��
	constexpr int kNoHitFrame = 30;
}

UltGageUp::UltGageUp(int modelHandle, Vector3 pos) :
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

UltGageUp::~UltGageUp()
{
}

void UltGageUp::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void UltGageUp::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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

void UltGageUp::OnCollide(const std::shared_ptr<Collidable> other)
{
	//���Ńt���O�������Ă�Ȃ烊�^�[��
	if (m_isDelete)return;
	//�v���C���[�ɓ����������̏���
	if (other->GetGameTag() == GameTag::Player)
	{
		//�Q�[�W�A�b�v
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetUltGage().lock()->AddUltGage(kGageValue);
		//�폜
		m_isDelete = true;
	}
}

void UltGageUp::Draw() const
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

void UltGageUp::Complete()
{
	//���̍��W��
	m_rb->m_pos = m_rb->GetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void UltGageUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillScore(ScoreDataName::kUltGageUp);
}

void UltGageUp::End()
{
	Collidable::End();
	m_model->End();
}

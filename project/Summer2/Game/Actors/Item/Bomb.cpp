#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"
#include "../Attack/Blast.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//�����܂ł̃t���[����
	constexpr int kBlastFrame = 180;
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 50.0f;
	//�ŏ��̓�����Ȃ��t���[��
	constexpr int kNoHitFrame = 10;

	//�������O�̃t���[��
	constexpr int kBeforeBlastFrame = 60;
	//���f���̑傫��
	const VECTOR kModelScale = { 0.7f, 0.7f, 0.7f };
	//�g��k���̑傫��
	const Vector3 kAddScaleSize = { 0.1f, 0.1f, 0.1f };
	//���f���̊g��k���̃X�s�[�h
	constexpr float kSlowScaleSpeed = 10.0f;
	constexpr float kFastScaleSpeed = 25.0f;

	//�����͈̔�
	constexpr float kBlastRadius = 170.0f;
	//�����̃_���[�W
	constexpr int kBlastDamage = 200;
	//�����̃m�b�N�o�b�N��
	constexpr float kBlastKnockBackPower = 25.0f;
	//�����̎�������
	constexpr float kBlastKeepFrame = 5.0f;
}

Bomb::Bomb(int modelHandle, Vector3 pos) :
	ItemBase(Shape::Sphere),
	m_blastCountFrame(kBlastFrame),
	m_scaleSpeed(0.0f),
	m_originalScale(kModelScale)
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

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void Bomb::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���ɂ���Ƃ�
	if (m_isFloor)
	{
		//�ړ��ʂ�������
		m_rb->SetMoveVec(Vector3::Zero());
	}
	//�����蔻��𖳎�����
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//�����蔻�������
		m_isThrough = false;
	}
	//�����܂ł̃J�E���g
	if (m_blastCountFrame > 0)
	{
		--m_blastCountFrame;
		//���e�̎���A�j���[�V����
		AnimBomb();
	}
	else
	{
		//����
		m_isDelete = true;
	}
	m_model->Update();
}


void Bomb::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Bomb::Draw() const
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

void Bomb::Complete()
{
	//���̍��W��
	m_rb->m_pos = m_rb->GetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void Bomb::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�����̍U��������o��
	auto attack = actorManager.lock()->CreateAttack(AttackType::Blast, std::dynamic_pointer_cast<Actor>(shared_from_this()));
	auto blast = std::dynamic_pointer_cast<Blast> (attack.lock());
	blast->SetPos(m_rb->m_pos);
	blast->SetRadius(kBlastRadius);
	blast->AttackSetting(kBlastDamage, kBlastKeepFrame, kBlastKnockBackPower, Battle::AttackWeight::Heavy);
}

void Bomb::End()
{
	Collidable::End();
	m_model->End();
}

void Bomb::SetVec(Vector3 vec)
{
	m_rb->m_vec = vec;
}

void Bomb::AnimBomb()
{
	if (m_blastCountFrame <= kBeforeBlastFrame)
	{
		//�����܂ł̎c�莞�Ԃ��Z���Ƃ������Ȃ�
		m_scaleSpeed += kFastScaleSpeed;
	}
	else
	{
		//����ȊO�͂������g��k��
		m_scaleSpeed += kSlowScaleSpeed;
	}
	Vector3 modelScale = m_originalScale + kAddScaleSize * cosf(m_scaleSpeed * MyMath::DEG_2_RAD);
	m_model->SetScale(modelScale.ToDxLibVector());
}
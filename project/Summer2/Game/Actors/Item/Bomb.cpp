#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../Player/Player.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/BlastAttack.h"
#include "../../Attack/AttackManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//���f���̑傫��
	const VECTOR kModelScale = { 0.5f, 0.5f, 0.5f };
	//�����蔻��̔��a
	constexpr float kCollRadius = 30.0f;
	//�����̓����蔻��̔��a
	constexpr float kBlastRadius = 170.0f;
	//�����܂ł̃t���[����
	constexpr int kBlastFrame = 180;
	//�������O�̃t���[��
	constexpr int kBeforeBlastFrame = 60;
	//���f���̊g��k���̃X�s�[�h
	constexpr float kSlowScaleSpeed = 10.0f;
	constexpr float kFastScaleSpeed = 25.0f;
	//�g��k���̑傫��
	const Vector3 kAddScaleSize = { 0.1f, 0.1f, 0.1f };
	//�����̃_���[�W
	constexpr int kBlastDamage = 200;
	//�����̃m�b�N�o�b�N��
	constexpr float kBlastKnockBackPower = 25.0f;
	//�����̎�������
	constexpr float kBlastKeepFrame = 5.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
}

Bomb::Bomb(int modelHandle, Vector3 pos, Vector3 vec) :
	ItemBase(ItemKind::Heart),
	m_blastCountFrame(kBlastFrame),
	m_scaleSpeed(0.0f)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//���f���̑傫����ݒ�
	m_model->SetScale(kModelScale);
	//�Փ˔���
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//�R���C�_�u���̏�����
	m_collidable->Init(State::None, Priority::Low, GameTag::Item);
	//���f���̃T�C�Y��ێ�
	m_originalScale = m_model->GetScale();
	//���f���̌�����180�x��]������
	m_model->SetRot({ 0.0f, 180.0f, 0.0f });
	//�ړ���
	m_collidable->GetRb()->SetVec(vec);
}

Bomb::~Bomb()
{
}

void Bomb::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
}

void Bomb::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
}

void Bomb::Init()
{
	//�R���C�_�[�Ɏ����̃|�C���^����������
	m_collidable->SetOwner(shared_from_this());
}

void Bomb::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�����̃J�E���g�����炷
	m_blastCountFrame--;
	//�����܂ł̃t���[����0�ɂȂ����甚��
	if (m_blastCountFrame <= 0 && !m_isDelete)
	{
		//�����̓����蔻����쐬
		CreateAttack();
		//�U����o�^
		actorManager->GetAttackManager()->Entry(m_blastAttack);
		//�폜�t���O�𗧂Ă�
		m_isDelete = true;
		return;
	}
	//���e�̃A�j���[�V����
	BombAnim();
	//����
	SpeedDown();
}

void Bomb::BombAnim()
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

void Bomb::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Bomb::OnHitColl(const std::shared_ptr<Collidable>& other)
{

}

void Bomb::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawSphere3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
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
	m_collidable->GetRb()->SetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Bomb::CreateAttack()
{
	std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->SetRadius(kBlastRadius);
	//�����̓����蔻����쐬
	m_blastAttack = std::make_shared<BlastAttack>(m_collidable, kBlastDamage, kBlastKeepFrame, kBlastKnockBackPower, shared_from_this());
}

void Bomb::SpeedDown()
{
	//����
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	m_collidable->GetRb()->SetVec(vec);
}

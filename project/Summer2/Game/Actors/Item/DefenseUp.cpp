#include "DefenseUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../Player/Player.h"
#include "../Player/UltGage.h"

namespace
{
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 50.0f;
	//��]��
	constexpr float kRotaAngle = 1.0f;
	//�ŏ��̓�����Ȃ��t���[��
	constexpr int kNoHitFrame = 30;

	//�_���[�W�J�b�g��
	constexpr float kDamageCutRate = 0.5f;
	//�����t���[��
	constexpr int kDamageCutKeepFrame = 60 * 15;//15�b
}

DefenseUp::DefenseUp(int modelHandle, Vector3 pos) :
	ItemBase(ItemKind::DefenseUp),
	m_noHitFrame(kNoHitFrame)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�Փ˔���
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//�͂�^����
	m_collidable->GetRb()->SetVecY(kJumpPower);
	//�R���C�_�u���̏�����
	m_collidable->Init(State::None, Priority::Low, GameTag::Item);
	//�����蔻������Ȃ�
	m_collidable->SetIsCollide(false);
}

DefenseUp::~DefenseUp()
{
}

void DefenseUp::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
}

void DefenseUp::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
}

void DefenseUp::Init()
{
	//�R���C�_�[�Ɏ����̃|�C���^����������
	m_collidable->SetOwner(shared_from_this());
}

void DefenseUp::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�ړ��ʂ�������
	m_collidable->GetRb()->SetMoveVec(Vector3::Zero());
	//���
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//�����蔻�������
		m_collidable->SetIsCollide(true);
	}
}

void DefenseUp::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void DefenseUp::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//���Ńt���O�������Ă�Ȃ烊�^�[��
	if (m_isDelete)return;
	//�v���C���[�ɓ����������̏���
	if (other->GetGameTag() == GameTag::Player)
	{
		//�_���[�W�J�b�g
		std::dynamic_pointer_cast<Player>(other->GetOwner())->SetDamageCut(kDamageCutRate, kDamageCutKeepFrame);
		//�폜
		m_isDelete = true;
	}
}

void DefenseUp::Draw() const
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

void DefenseUp::Complete()
{
	//���̍��W��
	m_collidable->GetRb()->SetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}


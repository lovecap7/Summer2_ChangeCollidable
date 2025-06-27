#include "Lariat.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
Lariat::Lariat(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius) :
	CapsuleAttackBase(ownerState, modelHandle, startIndex, endIndex, radius)
{
}

Lariat::~Lariat()
{
}

void Lariat::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Attack, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void Lariat::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (m_ownerState.expired())
	{
		m_isDelete = true; //���L�҂̃X�e�[�g�������Ȃ�����폜�t���O�𗧂Ă�
		return; //���L�҂̃X�e�[�g�������Ȃ����牽�����Ȃ�
	}
	//���f���̃C���f�b�N�X����ʒu�X�V
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	//�n�_�ƏI�_�̍��W
	VECTOR startPos = MV1GetFramePosition(m_modelHandle, m_startIndex);
	VECTOR endPos = MV1GetFramePosition(m_modelHandle, m_endIndex);
	//�X�V
	m_rb->SetPos(startPos);
	cap->SetEndPos(Vector3(endPos));
}

void Lariat::Draw() const
{
}

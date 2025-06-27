#include "Slash.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
Slash::Slash(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius, float swordLength):
	CapsuleAttackBase(ownerState, modelHandle,startIndex,endIndex,radius),
	m_swordLength(swordLength)
{
}

Slash::~Slash()
{
}

void Slash::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Attack, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void Slash::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//����̖���
	VECTOR swordDir = VNorm(VSub(startPos, startPos));
	swordDir = VScale(swordDir, m_swordLength);	//����̒���
	swordDir = VAdd(startPos, swordDir);		//������̍��W�ɉ��Z���Č���̍��W���o��
	//����̎�������Z�b�g
	m_rb->SetPos(startPos);
	//����̌�����Z�b�g
	cap->SetEndPos(swordDir);

	//�����t���[�������炷
	m_keepFrame--;
	//�����t���[����0�ɂȂ�����폜
	if (m_keepFrame <= 0)
	{
		m_isDelete = true; //�폜�t���O�𗧂Ă�
		return; //�������Ȃ�
	}
}

void Slash::Draw() const
{
}

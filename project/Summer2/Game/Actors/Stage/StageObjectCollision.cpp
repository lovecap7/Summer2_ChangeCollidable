#include "StageObjectCollision.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../ActorManager.h"

StageObjectCollision::StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle) :
	m_collisionHandle(modelHandle)
{
	//�ʒu�Ƒ傫���Ɖ�]�̃Z�b�g
	//�����ʒu
	m_collidable = std::make_shared<Collidable>(std::make_shared<PolygonCollider>(modelHandle), std::make_shared<Rigidbody>(pos));
	DxLib::MV1SetScale(m_collisionHandle, scale);
	DxLib::MV1SetRotationXYZ(m_collisionHandle, angle);
	//������
	m_collidable->Init(State::None,Priority::Static,GameTag::Object);
}

StageObjectCollision::StageObjectCollision(VECTOR pos, float radius):
	m_collisionHandle(-1)
{
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(radius),std::make_shared<Rigidbody>(pos));
	//������
	m_collidable->Init(State::None, Priority::Static, GameTag::Object);
}

StageObjectCollision::StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius):
	m_collisionHandle(-1)
{
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(pos2, radius), std::make_shared<Rigidbody>(pos1));
	//������
	m_collidable->Init(State::None, Priority::Static, GameTag::Object);
}


StageObjectCollision::~StageObjectCollision()
{
	//�Ȃ�
}

void StageObjectCollision::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
}

void StageObjectCollision::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
}

void StageObjectCollision::Init()
{
	//�R���C�_�[�Ɏ����̃|�C���^����������
	m_collidable->SetOwner(shared_from_this());
}

void StageObjectCollision::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�Ȃ�
}

void StageObjectCollision::Gravity(const Vector3& gravity)
{
	//�Ȃ�
}

void StageObjectCollision::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//�Ȃ�
}

void StageObjectCollision::Draw() const
{
	//�Ȃ�
}

void StageObjectCollision::Complete()
{
	//�Ȃ�
}

#include "StageObjectDraw.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"

StageObjectDraw::StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle)
{
	//�`��p�̃N���X
	m_isDrawOnly = true;
	//���f��
	m_model = std::make_shared<Model>(modelHandle, pos);
	m_model->SetScale(scale);
	m_model->SetRot(angle);
}


StageObjectDraw::~StageObjectDraw()
{
	//�Ȃ�
}

void StageObjectDraw::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
}

void StageObjectDraw::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
}

void StageObjectDraw::Init()
{
	//�Ȃ�
}

void StageObjectDraw::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�Ȃ�
}

void StageObjectDraw::Gravity(const Vector3& gravity)
{
	//�Ȃ�
}

void StageObjectDraw::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//�Ȃ�
}

void StageObjectDraw::Draw() const
{
	m_model->Draw();
}

void StageObjectDraw::Complete()
{
	//�Ȃ�
}

#include "StageObjectDraw.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Model.h"

StageObjectDraw::StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle):
	Actor(Shape::None)
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
}

void StageObjectDraw::Draw() const
{
	m_model->Draw();
}
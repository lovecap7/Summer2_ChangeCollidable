#include "PlayerStateBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"

PlayerStateBase::PlayerStateBase(std::weak_ptr<Actor>  player):
	CharacterStateBase(player)
{
}

PlayerStateBase::~PlayerStateBase()
{
}

Vector3 PlayerStateBase::GetForwardVec(const std::weak_ptr<Camera> camera)
{
	Vector3 rV = { 0.0f,0.0f,0.0f, };
	//�v���C���[
	auto player = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//����
	Vector3 stickVec = player->GetStickVec().XZ();
	if (stickVec.Magnitude() < 0.0f)
	{
		return rV;
	}
	//�J�����̌����ɂ��킹��
	//�J�����̌���
	Vector3 cameraDir = camera.lock()->GetDir();
	cameraDir.y = 0.0f;
	if (cameraDir.Magnitude() > 0.0f)
	{
		cameraDir = cameraDir.Normalize();
	}
	//��]�}�g���N�X���쐬
	Matrix4x4 cameraM = Matrix4x4::LookAt(cameraDir, Vector3::Up());
	Matrix4x4 stickM = Matrix4x4::LookAt(stickVec, Vector3::Up());
	//�x�N�g���ɂ�����(��])
	Vector3 moveVec = Vector3::Forward();
	moveVec = stickM * cameraM * moveVec;
	moveVec.y = 0.0f; //Y���͖���
	if (moveVec.Magnitude() > 0.0f)
	{
		rV = moveVec.Normalize();
	}
	return rV;
}
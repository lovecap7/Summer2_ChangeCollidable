#include "PlayerStateBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
PlayerStateBase::PlayerStateBase(std::shared_ptr<Player>  player):
	m_player(player)
{
}

PlayerStateBase::~PlayerStateBase()
{
}

void PlayerStateBase::ChangeState(std::shared_ptr<PlayerStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}

Vector3 PlayerStateBase::GetForwardVec(const Input& input, const std::unique_ptr<Camera>& camera)
{
	Vector3 rV = { 0.0f,0.0f,0.0f, };
	//����
	Vector3 stickVec = { m_player->GetStickVec().x,0.0f,m_player->GetStickVec().y };
	if (stickVec.Magnitude() < 0.0f)
	{
		return rV;
	}
	//�J�����̌����ɂ��킹��
	//�J�����̌���
	Vector3 cameraDir = camera->GetDir();
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
void PlayerStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager)
{
	//�U��������
	attack->Init();//��Ԃ����Z�b�g
	attackManager->Entry(attack);//�o�^
}

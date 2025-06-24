#include "Model.h"
#include "Math/MyMath.h"
#include "Animator.h"
#include <array>
#include <cassert>

namespace
{
	constexpr float kAnimSpeed = 0.5f;//�Đ����x
	//��]�t���[��
	constexpr int kRotaFrame = 10;
	//�q�b�g���ʃt���[��
	constexpr int kHitFrame = 30.0f;
	//�q�b�g���ʂŃ��f�����傫���Ȃ�{��
	constexpr float kHiScaleRate = 1.1f;
}

Model::Model(int modelHandle, VECTOR pos) :
	m_modelHandle(modelHandle),
	m_forward{ 0.0f,0.0f,1.0f },
	m_nextForward{ 0.0f,0.0f,1.0f },
	m_rotation(Quaternion::AngleAxis(180 * MyMath::DEG_2_RAD, Vector3::Up())),
	m_rotaQ(Quaternion::IdentityQ()),
	m_rotaFrame(0),
	m_pos(pos),
	m_scale{ 1.0f,1.0f,1.0f },
	m_hitCountFrame(0),
	m_diffColor{ 1.0f,1.0f ,1.0f ,1.0f },
	m_beforeScale(m_scale),
	m_beforeScaleDif{}
{
	//���W
	MV1SetPosition(m_modelHandle, pos);
	//�A�j���[�V����
	m_animator = std::make_unique<Animator>();
}

Model::Model(int modelHandle, VECTOR pos, Vector3 forward) :
	m_modelHandle(modelHandle),
	m_forward(forward),
	m_nextForward(forward),
	m_rotation(Quaternion::IdentityQ()),
	m_rotaQ(Quaternion::IdentityQ()),
	m_rotaFrame(0),
	m_pos(),
	m_scale{ 1.0f,1.0f,1.0f },
	m_hitCountFrame(0),
	m_diffColor{ 1.0f,1.0f ,1.0f ,1.0f },
	m_beforeScale(m_scale),
	m_beforeScaleDif{}
{
	//���W
	m_pos = pos;
	MV1SetPosition(m_modelHandle, m_pos.ToDxLibVector());
	//�A�j���[�V����
	m_animator = std::make_unique<Animator>();
}

Model::~Model()
{
}

void Model::Update()
{
	//�A�j���[�V�����̍X�V
	m_animator->PlayAnim(m_modelHandle);

	//�����̍X�V
	if (m_rotaFrame > 0)
	{
		--m_rotaFrame;
		//��]
		m_rotation = m_rotaQ * m_rotation;
		//���K��
		if (m_rotation.Magnitude() > 0.0f && m_rotaQ.w < 1.0f)
		{
			m_rotation = m_rotation.NormQ();
			MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
		}
		//�O�x�N�g��
		m_forward = m_rotaQ * m_forward;
		//���K��
		if (m_forward.Magnitude() > 0.0f)m_forward = m_forward.Normalize();
	}
	//�q�b�g���ʂ��猳�̏�Ԃɖ߂��Ă���
	if (m_hitCountFrame > 0)
	{
		--m_hitCountFrame;
		//���Ƃɖ߂��Ă�(�F)
		m_diffColor.g += 1.0f / kHitFrame;
		m_diffColor.b += 1.0f / kHitFrame;
		SetDiffuseColor(m_diffColor);
		//�傫��
		m_scale -= m_beforeScaleDif / kHitFrame;
	}
}

void Model::Draw() const
{
	Matrix4x4 mat;
	auto pMat = Matrix4x4::TranslateMat4x4(m_pos.x, m_pos.y, m_pos.z);
	auto rMat = m_rotation.GetMatrix();
	auto sMat = Matrix4x4::ScaleMatrix4x4(m_scale);
	mat = sMat * rMat * pMat;
	MV1SetMatrix(m_modelHandle, mat.ToDxLibMATRIX());
	//�`��
	auto err = MV1DrawModel(m_modelHandle);
	assert(err != -1 && L"���f�����`��ł��܂���");
	
#if _DEBUG
	//���Ă����
	auto forward = m_forward * 50.0f;
	auto pos = m_pos + forward;
	DrawSphere3D(pos.ToDxLibVector(), 20, 16, 0xffffff, 0xffffff, true);
#endif
}

void Model::SetPos(VECTOR pos)
{
	m_pos = pos;
}

void Model::SetScale(VECTOR scale)
{
	m_scale = scale;
	m_beforeScale = m_scale;
}

void Model::SetRot(VECTOR rot)
{
	//�I�C���[�p�����]
	m_rotaQ = Quaternion::Euler(rot.x, rot.y, rot.z);
	//��]
	m_rotation = m_rotaQ * m_rotation;
	//���K��
	if (m_rotation.Magnitude() > 0.0f && m_rotaQ.w < 1.0f)
	{
		m_rotation = m_rotation.NormQ();
		MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
	}
	//�O�x�N�g��
	m_forward = m_rotaQ * m_forward;
	//���K��
	if (m_forward.Magnitude() > 0.0f)m_forward = m_forward.Normalize();
}

void Model::SetDir(Vector2 vec)
{
	//���������߂��Ȃ��̂Ń��^�[��
	if (vec.Magnitude() <= 0.0f)	return;
	//�������v�Z
	Vector2 dir = vec;
	dir = dir.Normalize();
	//if (m_nextForward.XZ() == dir)return;//�������ς��Ȃ��Ȃ�
	float angle = Vector2::Theata(m_forward.XZ(), dir);
	Vector3 axis = m_forward.Cross(dir.XZ());
	if (axis.SqMagnitude() == 0.0f)
	{
		axis = Vector3::Up();
	}
	//��]�N�H�[�^�j�I���쐬
	m_rotaQ = Quaternion::AngleAxis(angle/ kRotaFrame, axis);
	//�t���[�����Z�b�g
	m_rotaFrame = kRotaFrame;
	//���̐��ʃx�N�g�����L�^
	m_nextForward = dir.XZ();
}

void Model::SetDiffuseColor(float r, float g, float b, float a)
{
	COLOR_F color = { r, g, b, a };
	m_diffColor = color;
	MV1SetDifColorScale(m_modelHandle, m_diffColor);
}

void Model::SetDiffuseColor(COLOR_F color)
{
	m_diffColor = color;
	MV1SetDifColorScale(m_modelHandle, m_diffColor);
}

Vector3 Model::GetDir()
{
	Vector3 dir = m_forward;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}
void Model::ModelHit()
{
	//�Ԃ�
	SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	//�t���[�����Z�b�g
	m_hitCountFrame = kHitFrame;
	//�����傫������
	m_scale = m_beforeScale;						//��U���̑傫��
	m_scale *= kHiScaleRate;						//�傫������
	m_beforeScaleDif = (m_scale - m_beforeScale);	//�����v�Z

}
//
//void Model::DrawBoundingBox()const
//{
//	//�o�E���f�B���O�{�b�N�X
//	std::array<VECTOR, 8>bbVertices;
//	auto vmax = MV1GetFrameMaxVertexLocalPosition(m_modelHandle,0);
//	auto vmin = MV1GetFrameMinVertexLocalPosition(m_modelHandle, 0);
//
//	//�傫������
//	/*VECTOR scale = MV1GetScale(m_modelHandle);
//	vmax = VScale(vmax, VSize(scale));
//	vmin = VScale(vmin, VSize(scale));*/
//
//	bbVertices[0] = vmin;
//	bbVertices[1] = { vmax.x,vmin.y,vmin.z };
//	bbVertices[2] = { vmin.x,vmax.y,vmin.z };
//	bbVertices[3] = { vmax.x,vmax.y,vmin.z };
//	bbVertices[4] = { vmin.x,vmin.y,vmax.z };
//	bbVertices[5] = { vmax.x,vmin.y,vmax.z };
//	bbVertices[6] = { vmin.x,vmax.y,vmax.z };
//	bbVertices[7] = { vmax.x,vmax.y,vmax.z };
//	for (auto& v : bbVertices)
//	{
//		DrawSphere3D(v, 2.0f, 16, 0xffffff, 0, false);
//	}
//}

void Model::SetAnim(const char* animName, bool isLoop)
{
	//�A�j���[�V������ύX
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop);
}

void Model::SetAnim(const char* animName, bool isLoop, const float& animSpeed)
{
	//�A�j���[�V������ύX
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop, animSpeed);
}

void Model::SetAnimSpeed(const float& animSpeed)
{
	//�A�j���[�V�����̑��x
	m_animator->SetAnimSpeed(animSpeed);
}

bool Model::IsFinishAnim()
{
	//�A�j���[�V�������I��������ǂ���
	return m_animator->IsFinishAnim();
}

float Model::GetTotalAnimFrame()
{
	//�S�̃t���[��
	return m_animator->GetTotalAnimFrame();
}

void Model::SetTotalAnimFrame(float totalFrame)
{
	//�S�̃t���[���Z�b�g
	m_animator->SetTotalAnimFrame(totalFrame);
}

float Model::GetNowAnimFrame()
{
	//���݂̃t���[��
	return m_animator->GetAnimFrame();
}

void Model::ReplayAnim()
{
	//�A�j���[�V�������Đ�
	m_animator->Replay();
}

void Model::DeleteAnim()
{
	//�폜
	m_animator->AllRemoveAnim(m_modelHandle);
}

void Model::SetFixedLoopFrame(float loopFrame)
{
	//�w�胋�[�v
	m_animator->SetFixedLoop(loopFrame);
}

bool Model::IsFinishFixedLoop()
{
	//�w�胋�[�v�I��
	return m_animator->IsFinishFixedLoop();
}

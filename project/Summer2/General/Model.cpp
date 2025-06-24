#include "Model.h"
#include "Math/MyMath.h"
#include "Animator.h"
#include <array>
#include <cassert>

namespace
{
	constexpr float kAnimSpeed = 0.5f;//再生速度
	//回転フレーム
	constexpr int kRotaFrame = 10;
	//ヒット効果フレーム
	constexpr int kHitFrame = 30.0f;
	//ヒット効果でモデルが大きくなる倍率
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
	//座標
	MV1SetPosition(m_modelHandle, pos);
	//アニメーション
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
	//座標
	m_pos = pos;
	MV1SetPosition(m_modelHandle, m_pos.ToDxLibVector());
	//アニメーション
	m_animator = std::make_unique<Animator>();
}

Model::~Model()
{
}

void Model::Update()
{
	//アニメーションの更新
	m_animator->PlayAnim(m_modelHandle);

	//向きの更新
	if (m_rotaFrame > 0)
	{
		--m_rotaFrame;
		//回転
		m_rotation = m_rotaQ * m_rotation;
		//正規化
		if (m_rotation.Magnitude() > 0.0f && m_rotaQ.w < 1.0f)
		{
			m_rotation = m_rotation.NormQ();
			MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
		}
		//前ベクトル
		m_forward = m_rotaQ * m_forward;
		//正規化
		if (m_forward.Magnitude() > 0.0f)m_forward = m_forward.Normalize();
	}
	//ヒット効果から元の状態に戻していく
	if (m_hitCountFrame > 0)
	{
		--m_hitCountFrame;
		//もとに戻してく(色)
		m_diffColor.g += 1.0f / kHitFrame;
		m_diffColor.b += 1.0f / kHitFrame;
		SetDiffuseColor(m_diffColor);
		//大きさ
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
	//描画
	auto err = MV1DrawModel(m_modelHandle);
	assert(err != -1 && L"モデルが描画できません");
	
#if _DEBUG
	//見てる方向
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
	//オイラー角から回転
	m_rotaQ = Quaternion::Euler(rot.x, rot.y, rot.z);
	//回転
	m_rotation = m_rotaQ * m_rotation;
	//正規化
	if (m_rotation.Magnitude() > 0.0f && m_rotaQ.w < 1.0f)
	{
		m_rotation = m_rotation.NormQ();
		MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
	}
	//前ベクトル
	m_forward = m_rotaQ * m_forward;
	//正規化
	if (m_forward.Magnitude() > 0.0f)m_forward = m_forward.Normalize();
}

void Model::SetDir(Vector2 vec)
{
	//向きが決められないのでリターン
	if (vec.Magnitude() <= 0.0f)	return;
	//向きを計算
	Vector2 dir = vec;
	dir = dir.Normalize();
	//if (m_nextForward.XZ() == dir)return;//向きが変わらないなら
	float angle = Vector2::Theata(m_forward.XZ(), dir);
	Vector3 axis = m_forward.Cross(dir.XZ());
	if (axis.SqMagnitude() == 0.0f)
	{
		axis = Vector3::Up();
	}
	//回転クォータニオン作成
	m_rotaQ = Quaternion::AngleAxis(angle/ kRotaFrame, axis);
	//フレームをセット
	m_rotaFrame = kRotaFrame;
	//次の正面ベクトルを記録
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
	//赤に
	SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	//フレームをセット
	m_hitCountFrame = kHitFrame;
	//少し大きくする
	m_scale = m_beforeScale;						//一旦元の大きさ
	m_scale *= kHiScaleRate;						//大きくする
	m_beforeScaleDif = (m_scale - m_beforeScale);	//差を計算

}
//
//void Model::DrawBoundingBox()const
//{
//	//バウンディングボックス
//	std::array<VECTOR, 8>bbVertices;
//	auto vmax = MV1GetFrameMaxVertexLocalPosition(m_modelHandle,0);
//	auto vmin = MV1GetFrameMinVertexLocalPosition(m_modelHandle, 0);
//
//	//大きさ調整
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
	//アニメーションを変更
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop);
}

void Model::SetAnim(const char* animName, bool isLoop, const float& animSpeed)
{
	//アニメーションを変更
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop, animSpeed);
}

void Model::SetAnimSpeed(const float& animSpeed)
{
	//アニメーションの速度
	m_animator->SetAnimSpeed(animSpeed);
}

bool Model::IsFinishAnim()
{
	//アニメーションが終わったかどうか
	return m_animator->IsFinishAnim();
}

float Model::GetTotalAnimFrame()
{
	//全体フレーム
	return m_animator->GetTotalAnimFrame();
}

void Model::SetTotalAnimFrame(float totalFrame)
{
	//全体フレームセット
	m_animator->SetTotalAnimFrame(totalFrame);
}

float Model::GetNowAnimFrame()
{
	//現在のフレーム
	return m_animator->GetAnimFrame();
}

void Model::ReplayAnim()
{
	//アニメーションを再生
	m_animator->Replay();
}

void Model::DeleteAnim()
{
	//削除
	m_animator->AllRemoveAnim(m_modelHandle);
}

void Model::SetFixedLoopFrame(float loopFrame)
{
	//指定ループ
	m_animator->SetFixedLoop(loopFrame);
}

bool Model::IsFinishFixedLoop()
{
	//指定ループ終了
	return m_animator->IsFinishFixedLoop();
}

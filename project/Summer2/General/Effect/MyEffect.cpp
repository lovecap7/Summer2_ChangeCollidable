#include "MyEffect.h"
#include <EffekseerForDXLib.h>
namespace
{
	constexpr float kDefaultScale = 100.0f;
}

MyEffect::MyEffect(int playHandle, Vector3 pos):
	m_playHandle(playHandle),
	m_pos(pos),
	m_rot{},
	m_scale{ kDefaultScale,kDefaultScale,kDefaultScale },
	m_playSpeed(1.0f),
	m_isDelete(false),
	m_specificFrame(0),
	m_isSpecificFrame(false),
	m_dir{0.0f,0.0f,-1.0f}
{
	SetPos(m_pos);
	SetRot(m_rot);
	SetScale(m_scale);
}

MyEffect::~MyEffect()
{
	StopEffekseer3DEffect(m_playHandle);
}

void MyEffect::Update()
{
	//指定フレーム再生なら
	if (m_isSpecificFrame)
	{
		if (m_specificFrame <= 0)
		{
			Delete();
			return;
		}
		--m_specificFrame;
	}
	else
	{
		//再生終了したら
		if (IsEffekseer3DEffectPlaying(m_playHandle) != 0)
		{
			Delete();
			return;
		}
	}
}

void MyEffect::End()
{
	Delete();
}

void MyEffect::Play()
{
	SetSpeedPlayingEffekseer3DEffect(m_playHandle, m_playSpeed);
}

void MyEffect::Stop()
{
	SetSpeedPlayingEffekseer3DEffect(m_playHandle, 0.0f);
}

void MyEffect::SetPlaySpeed(float speed)
{
	m_playHandle = speed;
	SetSpeedPlayingEffekseer3DEffect(m_playHandle, m_playSpeed);
}

void MyEffect::Delete()
{
	StopEffekseer3DEffect(m_playHandle);
	m_isDelete = true;
}

void MyEffect::SetPos(Vector3 pos)
{
	m_pos = pos;
	SetPosPlayingEffekseer3DEffect(m_playHandle,
		m_pos.x, m_pos.y, m_pos.z);
}

void MyEffect::SetRot(Vector3 rot)
{
	m_rot = rot;
	//おそらく相対ではなく絶対
	SetRotationPlayingEffekseer3DEffect(m_playHandle,
		m_rot.x, m_rot.y, m_rot.z);
}

void MyEffect::SetScale(Vector3 scale)
{
	m_scale = scale;
	SetScalePlayingEffekseer3DEffect(m_playHandle,
		m_scale.x, m_scale.y, m_scale.z);
}

void MyEffect::LookAt(Vector3 dir)
{
	auto nextDir = dir;
	//正規化
	if (nextDir.SqMagnitude() > 0.0f)
	{
		nextDir = nextDir.Normalize();
	}
	//Y角度
	float yRad = Vector2::GetRad(m_dir.XZ(), nextDir.XZ());
	//外積から回転方向を計算
	float cross = m_dir.XZ().Cross(nextDir.XZ());
	if (cross > 0)
	{
		yRad *= -1;
	}
	//角度をセット
	Vector3 rot = { 0.0f,yRad,0.0f};
	SetRot(rot);
}

void MyEffect::SpecificFrame(int frame)
{
	m_specificFrame = frame;
	m_isSpecificFrame = true;
}

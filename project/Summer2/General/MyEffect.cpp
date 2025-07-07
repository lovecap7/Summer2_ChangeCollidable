#include "MyEffect.h"
#include <EffekseerForDXLib.h>

MyEffect::MyEffect(int playHandle, Vector3 pos):
	m_playHandle(playHandle),
	m_pos(pos),
	m_rot{},
	m_scale{1.0f,1.0f,1.0f},
	m_playSpeed(1.0f),
	m_isDelete(false)
{
}

MyEffect::~MyEffect()
{
}

void MyEffect::Update()
{
	//çƒê∂èIóπÇµÇΩÇÁ
	if (IsEffekseer3DEffectPlaying(m_playHandle) != 0)
	{
		Delete();
		return;
	}
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
	SetRotationPlayingEffekseer3DEffect(m_playHandle,
		m_rot.x, m_rot.y, m_rot.z);
}

void MyEffect::SetScale(Vector3 scale)
{
	m_scale = scale;
	SetScalePlayingEffekseer3DEffect(m_playHandle,
		m_scale.x, m_scale.y, m_scale.z);
}

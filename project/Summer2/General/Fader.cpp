#include "Fader.h"
#include "game.h"
#include <DxLib.h>
//‰Šú‰»
void Fader::Init()
{
	m_isFadeIn = false;
	m_isFadeOut = false;
	m_countFrame = 0.0f;
	m_fadeSpeed = 10.0f;
}

//XV
void Fader::Update()
{
	if (m_isFadeIn)
	{
		//‚¾‚ñ‚¾‚ñ–¾‚é‚­‚È‚é
		m_countFrame -= m_fadeSpeed;
		if (m_countFrame <= 0.0f)
		{
			m_countFrame = 0.0f;
			m_isFadeIn = false;
		}
	}
	if (m_isFadeOut)
	{
		//‚¾‚ñ‚¾‚ñˆÃ‚­‚È‚é
		m_countFrame += m_fadeSpeed;
		if (m_countFrame >= 255.0f)
		{
			m_countFrame = 255.0f;
			m_isFadeOut = false;
		}
	}
}
//•`‰æ
void Fader::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_countFrame);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void Fader::FadeIn(float speed)
{
	if (!m_isFadeIn)
	{
		//‚¾‚ñ‚¾‚ñ–¾‚é‚­‚È‚é
		m_countFrame = 255.0f;
		m_fadeSpeed = speed;
		m_isFadeIn = true;
		m_isFadeOut = false;
	}
}

void Fader::FadeOut(float speed)
{
	if (!m_isFadeOut)
	{
		//‚¾‚ñ‚¾‚ñˆÃ‚­‚È‚é
		m_countFrame = 0.0f;
		m_fadeSpeed = speed;
		m_isFadeOut = true;
		m_isFadeIn = false;
	}
}

bool Fader::IsFinishFadeIn()const
{
	return m_countFrame <= 0.0f && !m_isFadeOut;
}


bool Fader::IsFinishFadeOut() const
{
	return  m_countFrame >= 255.0f && !m_isFadeIn;
}
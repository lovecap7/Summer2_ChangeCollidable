#include "ScoreUI.h"
#include "../../General/Math/MathSub.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>

namespace
{
	//�X�R�A�̌���
	constexpr int kDigitNum = 6;
	//

	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
}

ScoreUI::ScoreUI(int handle, const std::weak_ptr<Score> score) :
	UIBase(UIData::Score,handle),
	m_viewScore(0),
	m_viewMaxScore(0),
	m_score(score)
{
}

ScoreUI::~ScoreUI()
{
}

void ScoreUI::Init()
{
}

void ScoreUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�v���C���[�܂��̓{�X���������ꍇ�͂���UI���폜
	if (actorManager.lock()->GetPlayer().expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	//�X�R�A�X�V
	m_viewMaxScore = m_score.lock()->GetScore();
	if (m_viewScore != m_viewMaxScore)
	{
		auto score = MathSub::Lerp(static_cast<float>(m_viewScore), static_cast<float>(m_viewMaxScore), 0.5f);
		if (score + 1.0f >= static_cast<float>(m_viewMaxScore))
		{
			score = m_viewMaxScore;
		}
		m_viewScore = score;
	}
}

void ScoreUI::Draw() const
{
	//���o�����̈�
	int m_digits[kDigitNum];
	//���o���l
	int score = m_viewScore;
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		m_digits[i] = score % 10;
		//����������
		score = score / 10;
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = m_digits[i] % (sizeX / kImageWidth);//��
		int cutY = m_digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast((Game::kScreenWidth - 100) - i * 50, 50.0f,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			0.3f, 0.0f, m_handle, true, false);
	}
}

void ScoreUI::End()
{
	DeleteGraph(m_handle);
}

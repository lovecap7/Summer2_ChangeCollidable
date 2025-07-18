#include "ScoreUI.h"
#include "../../General/Math/MathSub.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>

namespace
{
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
	DrawFormatString((Game::kScreenWidth / 2.0f) - 100.0f, 50.0f, 0xffffff, "SCORE : %6d", m_viewScore);
	//���o�����̈�
	std::list<int> m_digits;
	//���o���l
	int score = m_viewScore;
	while (score > 0)
	{
		//���o���ĕۑ�
		m_digits.emplace_back(score % 10);
		//����������
		score = score / 10;
	}
	//for(auto m_digits)
	//{
	//	//�]�肩�琔�����m�F����
	//	auto num = m_viewScore % 10 * digit;
	//	//�؂�����v�Z����
	//	int sizeX, sizeY;
	//	GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
	//	int cutX = num % (sizeX / kImageWidth);//��
	//	int cutY = num / (sizeX / kImageWidth);//�c
	//	//�`��
	//	DrawRectRotaGraphFast((Game::kScreenWidth / 2.0f) - i * 30, 50.0f,
	//		kImageWidth * cutX,
	//		kImageHeight * cutY,
	//		kImageWidth, kImageHeight,
	//		0.1f, 0.0f, m_handle, true, false);
	//	//����������
	//	digit *= 10;
	//}
}

void ScoreUI::End()
{
	DeleteGraph(m_handle);
}

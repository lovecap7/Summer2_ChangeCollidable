#include "ScoreUI.h"
#include "../../General/Math/MathSub.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>

ScoreUI::ScoreUI(const std::weak_ptr<Score> score) :
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
	//プレイヤーまたはボスが消えた場合はこのUIも削除
	if (actorManager.lock()->GetPlayer().expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	//スコア更新
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
}

void ScoreUI::End()
{
}

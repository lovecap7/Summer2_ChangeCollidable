#include "Score.h"
#include "../../General/Math/MathSub.h"
#include "../../General/HitPoints.h"

Score::Score():
	m_nowScore(0),
	m_timeScore(0),
	m_killScore(0),
	m_itemScore(0),
	m_hpScore(0),
	m_hpScoreData(0),
	m_timeScoreData(0),
	m_decTimeScoreData(0)
{
	m_scoreData = CSVDataLoader::LoadScoreDataCSV("Data/CSV/ScoreData.csv");
	//ÉfÅ[É^Ç…àÍívÇ∑ÇÈÇ‡ÇÃÇíTÇ∑
	for (auto data : m_scoreData)
	{
		if (data.dataName == ScoreDataName::kHPScore)
		{
			m_hpScoreData = data.score;
		}
		else if (data.dataName == ScoreDataName::kTimeScore)
		{
			m_timeScoreData = data.score;
		}
		else if (data.dataName == ScoreDataName::kDecTimeScore)
		{
			m_decTimeScoreData = data.score;
		}
	}
}

Score::~Score()
{
}

int Score::GetScore()
{
	m_nowScore = m_timeScore + m_killScore + m_itemScore + m_hpScore;
	return m_nowScore;
}

void Score::AddTimeScore(int time)
{
	m_timeScore = m_timeScoreData - (m_decTimeScoreData * time);
}

void Score::AddKillScore(std::string dataName)
{
	//ÉfÅ[É^Ç…àÍívÇ∑ÇÈÇ‡ÇÃÇíTÇ∑
	for (auto data : m_scoreData)
	{
		if (data.dataName == dataName)
		{
			m_killScore += data.score;
			break;
		}
	}
}

void Score::AddHPScore(std::weak_ptr<HitPoints> hp)
{
	auto hitPoint = hp.lock();
	float nowHP = static_cast<float>(hitPoint->GetHp());
	float maxHP = static_cast<float>(hitPoint->GetMaxHp());
	m_hpScore = m_hpScoreData * (nowHP / maxHP);
}

#include "Score.h"
#include "../../General/Math/MathSub.h"
#include "../../General/HitPoints.h"

Score::Score()
{
	//スコアデータ
	m_csvLoader = std::make_unique<CSVDataLoader>();
	m_scoreData = m_csvLoader->LoadScoreDataCSV("Data/CSV/ScoreData.csv");
}

Score::~Score()
{
}

void Score::Init()
{
	m_nowScore = 0;
	m_timeScore = 0;
	m_killScore = 0;
	m_itemScore = 0;
	m_hpScore = 0;
	m_hpScoreData = 0;
	m_timeScoreData = 0;
	m_decTimeScoreData = 0;
	//データに一致するものを探す
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

int Score::GetScore()
{
	m_nowScore = m_timeScore + m_killScore + m_itemScore + m_hpScore;
	return m_nowScore;
}

void Score::AddTimeScore(int time)
{
	m_timeScore = m_timeScoreData - (m_decTimeScoreData * time);
	m_timeScore = MathSub::ClampInt(m_timeScore, 0, m_timeScoreData);
}

void Score::AddKillOrItemScore(std::string dataName)
{
		//データに一致するものを探す
		for (auto data : m_scoreData)
		{
			//見つかった時
			if (dataName == data.dataName)
			{
				//敵なら
				if (dataName == ScoreDataName::kPurpleDinosaur ||
					dataName == ScoreDataName::kSmallDragon ||
					dataName == ScoreDataName::kBomber ||
					dataName == ScoreDataName::kBossDragon)
				{
					m_killScore += data.score;
					break;
				}
				//アイテムなら
				else if (dataName == ScoreDataName::kAttackUp ||
					dataName == ScoreDataName::kDefenseUp ||
					dataName == ScoreDataName::kHeart ||
					dataName == ScoreDataName::kUltGageUp)
				{
					m_itemScore += data.score;
					break;
				}
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

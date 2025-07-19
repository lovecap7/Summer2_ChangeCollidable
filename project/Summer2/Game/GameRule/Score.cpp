#include "Score.h"
#include "../../General/Math/MathSub.h"
#include "../../General/HitPoints.h"
#include "../../General/CSVDataSaver.h"

Score::Score()
{
	//�X�R�A�f�[�^
	m_csvLoader = std::make_unique<CSVDataLoader>();
	m_acotrScoreData = m_csvLoader->LoadActorScoreDataCSV();
	//�n�C�X�R�A
	m_highScore = m_csvLoader->LoadHighScoreDataCSV();

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
	m_isUpdateHighScore = false;
	//�f�[�^�Ɉ�v������̂�T��
	for (auto data : m_acotrScoreData)
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

int Score::GetHighScore()
{
	//�n�C�X�R�A���X�V�����Ȃ�
	if (m_nowScore > m_highScore[0])
	{
		m_highScore[0] = m_nowScore;
		//�X�V�����̂�
		m_isUpdateHighScore = true;
	}
	return  m_highScore[0];
}

void Score::AddTimeScore(int time)
{
	m_timeScore = m_timeScoreData - (m_decTimeScoreData * time);
	m_timeScore = MathSub::ClampInt(m_timeScore, 0, m_timeScoreData);
}

void Score::AddKillOrItemScore(std::string dataName)
{
		//�f�[�^�Ɉ�v������̂�T��
		for (auto data : m_acotrScoreData)
		{
			//����������
			if (dataName == data.dataName)
			{
				//�G�Ȃ�
				if (dataName == ScoreDataName::kPurpleDinosaur ||
					dataName == ScoreDataName::kSmallDragon ||
					dataName == ScoreDataName::kBomber ||
					dataName == ScoreDataName::kBossDragon)
				{
					m_killScore += data.score;
					break;
				}
				//�A�C�e���Ȃ�
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

void Score::SaveHighScore()
{
	//�n�C�X�R�A���X�V�����Ȃ�
	if (m_isUpdateHighScore)
	{
		//�ۑ�����
		auto saver = std::make_shared<CSVDataSaver>();
		saver->SaveDataToCSV(shared_from_this());
	}
}

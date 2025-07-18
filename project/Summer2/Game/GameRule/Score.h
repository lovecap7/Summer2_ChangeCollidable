#pragma once
#include <memory>
#include <vector>
#include "../../General/CSVDataLoader.h"
class HitPoints;
class CSVDataLoader;
class Score
{
public:
	Score();
	~Score();
	//������
	void Init();
	//�X�R�A
	int GetScore();
	int GetTimeScore()	{ return m_timeScore; };
	int GetKillScore()	{ return m_killScore; };
	int GetItemScore()	{ return m_itemScore; };
	int GetHPScore()	{ return m_hpScore; };
	//�X�R�A���Z
	void AddTimeScore(int time);
	void AddKillOrItemScore(std::string dataName);
	void AddHPScore(std::weak_ptr<HitPoints> hp);
private:
	//���݂̃X�R�A
	int m_nowScore;	
	//���ԃX�R�A
	int m_timeScore;
	//���j�X�R�A(�G��|�����Ƃ��̃X�R�A)
	int m_killScore;
	//�A�C�e���X�R�A
	int m_itemScore;
	//�̗̓X�R�A
	int m_hpScore;
	//�X�R�A�f�[�^
	std::vector<ScoreData> m_scoreData;
	//�̗̓X�R�A�f�[�^
	int m_hpScoreData;
	//�^�C�}�[�X�R�A�f�[�^
	int m_timeScoreData;
	//�^�C�}�[�̌����ʃf�[�^
	int m_decTimeScoreData;
	//CSV
	std::unique_ptr<CSVDataLoader> m_csvLoader;
};
namespace ScoreDataName
{
	const std::string kHPScore			= "HPScore";
	const std::string kTimeScore		= "TimeScore";
	const std::string kDecTimeScore		= "DecTimeScore";
	const std::string kPurpleDinosaur	= "PurpleDinosaur";
	const std::string kSmallDragon		= "SmallDragon";
	const std::string kBomber			= "Bomber";
	const std::string kBossDragon		= "BossDragon";
	const std::string kAttackUp			= "AttackUp";
	const std::string kDefenseUp		= "DefenseUp";
	const std::string kHeart			= "Heart";
	const std::string kUltGageUp		= "UltGageUp";
}



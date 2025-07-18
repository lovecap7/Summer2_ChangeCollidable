#pragma once
#include <memory>
#include <vector>
#include "../../General/CSVDataLoader.h"
class HitPoints;
class CSVDataLoader;
class Score :
	public std::enable_shared_from_this<Score>
{
public:
	Score();
	~Score();
	//初期化
	void Init();
	//スコア
	int GetScore();
	int GetTimeScore()	{ return m_timeScore; };
	int GetKillScore()	{ return m_killScore; };
	int GetItemScore()	{ return m_itemScore; };
	int GetHPScore()	{ return m_hpScore; };
	int GetHighScore();
	//ハイスコアを更新したか
	bool IsUpdateHighScore()	{ return m_isUpdateHighScore; };
	//スコア加算
	void AddTimeScore(int time);
	void AddKillOrItemScore(std::string dataName);
	void AddHPScore(std::weak_ptr<HitPoints> hp);
	//スコアを保存
	void SaveHighScore();
private:
	//現在のスコア
	int m_nowScore;	
	//時間スコア
	int m_timeScore;
	//撃破スコア(敵を倒したときのスコア)
	int m_killScore;
	//アイテムスコア
	int m_itemScore;
	//体力スコア
	int m_hpScore;
	//ハイスコア
	std::vector<int> m_highScore;
	//ハイスコアを更新したなら
	bool m_isUpdateHighScore;
	//スコアデータ
	std::vector<ScoreData> m_acotrScoreData;
	//体力スコアデータ
	int m_hpScoreData;
	//タイマースコアデータ
	int m_timeScoreData;
	//タイマーの減少量データ
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



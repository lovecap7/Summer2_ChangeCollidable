#pragma once
#include "CSVPath.h"
#include <string>
#include <memory>
class Score;
/// <summary>
/// 保存したいデータをCSVに保存する
/// </summary>
class CSVDataSaver
{
public:
	CSVDataSaver();
	~CSVDataSaver();
	/// <summary>
	/// スコアを保存する
	/// </summary>
	/// <param name="score"></param>
	void SaveDataToCSV(std::shared_ptr<Score> score);
};


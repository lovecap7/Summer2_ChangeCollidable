#pragma once
#include "CSVPath.h"
#include <string>
#include <memory>
class Score;
/// <summary>
/// �ۑ��������f�[�^��CSV�ɕۑ�����
/// </summary>
class CSVDataSaver
{
public:
	CSVDataSaver();
	~CSVDataSaver();
	/// <summary>
	/// �X�R�A��ۑ�����
	/// </summary>
	/// <param name="score"></param>
	void SaveDataToCSV(std::shared_ptr<Score> score);
};


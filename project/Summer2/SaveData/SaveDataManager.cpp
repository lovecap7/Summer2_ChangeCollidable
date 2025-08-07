#include "SaveDataManager.h"
#include "../General/CSVDataLoader.h"
#include "../General/CSVDataSaver.h"
#include "../Game/GameRule/Score.h"

void SaveDataManager::Init()
{
	//スコア
	m_score = std::make_shared<Score>();
	//スコアの初期化
	m_score->Init();
	//データのロード
	Load();
}

void SaveDataManager::Update()
{

}

void SaveDataManager::End()
{
	Save();
}
void SaveDataManager::Load()
{
	auto csvLoader = std::make_shared<CSVDataLoader>();
	auto saveDatas = csvLoader->LoadSaveDataCSV();
	//データを取り出す
	m_isClearStage[Stage::StageIndex::Stage1] = saveDatas.stage1Clear;
	m_isClearStage[Stage::StageIndex::Stage2] = saveDatas.stage2Clear;
	m_isClearStage[Stage::StageIndex::Stage3] = saveDatas.stage3Clear;
	m_totalPlayTime							  = saveDatas.totalPlayTime;
}

void SaveDataManager::Save()
{
	//記録するデータを作る
	SaveDatas saveDatas;
	saveDatas.stage1Clear = m_isClearStage[Stage::StageIndex::Stage1];
	saveDatas.stage2Clear = m_isClearStage[Stage::StageIndex::Stage2];
	saveDatas.stage3Clear = m_isClearStage[Stage::StageIndex::Stage3];
	saveDatas.totalPlayTime = m_totalPlayTime;
	//CSVに書き込む
	auto csvSaver = std::make_shared<CSVDataSaver>();
	csvSaver->SaveDataToCSV(saveDatas);
}

void SaveDataManager::NewGame()
{
	//全てのデータを削除する
	m_isClearStage[Stage::StageIndex::Stage1] = false;
	m_isClearStage[Stage::StageIndex::Stage2] = false;
	m_isClearStage[Stage::StageIndex::Stage3] = false;
	m_totalPlayTime = 0;
	//CSVに書き込む
	Save();
}

void SaveDataManager::SaveClearStage(Stage::StageIndex stageIndex)
{
	//スコア更新
	m_score->UpdateScore(stageIndex);
	//ハイスコアを保存
	m_score->SaveHighScore();
	//クリアした
	m_isClearStage[stageIndex] = true;
	//記録
	Save();
}

bool SaveDataManager::IsClearStage(Stage::StageIndex stageIndex)
{
	return m_isClearStage[stageIndex];
}

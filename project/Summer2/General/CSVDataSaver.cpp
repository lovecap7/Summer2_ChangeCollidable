#include "CSVDataSaver.h"
#include <iostream>
#include <fstream>
#include <list>
#include <cassert>
#include "../Game/GameRule/Score.h"

CSVDataSaver::CSVDataSaver()
{
}

CSVDataSaver::~CSVDataSaver()
{
}

void CSVDataSaver::SaveDataToCSV(std::shared_ptr<Score> score)
{
    //ファイルを開く(ない場合は作成される)
    std::ofstream file(kHighScoreDataPath);
    //ファイルが読み込めなかったら
    if (!file)
    {
        assert("ファイルの読み込みに失敗");
    }
    
    //ヘッダーを書き込む
    file << "1位,2位,3位,Stage\n";
    //各ステージのランキングを記録
    for (int i = static_cast<int>(Stage::StageIndex::Stage1);i < static_cast<int>(Stage::StageIndex::StageNum);++i)
    {
        auto highScore = score->GetHighScore(static_cast<Stage::StageIndex>(i));
        //データ書き込み
        file << highScore[0] << ","
             << highScore[1] << ","
             << highScore[2] << ","
             << "Stage" << i + 1 << std::endl;
    }
    //閉じる
    file.close();
}

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
    file << "HighScore\n";
    //データ書き込み
    file << score->GetScore();
    //閉じる
    file.close();
}

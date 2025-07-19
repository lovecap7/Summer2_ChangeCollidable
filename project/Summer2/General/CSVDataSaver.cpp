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
    //�t�@�C�����J��(�Ȃ��ꍇ�͍쐬�����)
    std::ofstream file(kHighScoreDataPath);
    //�t�@�C�����ǂݍ��߂Ȃ�������
    if (!file)
    {
        assert("�t�@�C���̓ǂݍ��݂Ɏ��s");
    }
    //�w�b�_�[����������
    file << "HighScore\n";
    //�f�[�^��������
    file << score->GetScore();
    //����
    file.close();
}

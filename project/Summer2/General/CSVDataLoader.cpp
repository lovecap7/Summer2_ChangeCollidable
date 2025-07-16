#include "CSVDataLoader.h"
#include <fstream>   // �t�@�C���ǂݍ��ݗp
#include <sstream>   // �����񕪉�p�istringstream�jstring���t�@�C���̂悤�Ɉ�����

namespace
{
	//���O�A���WXYZ�A��]XYZ�A�傫��XYZ�@�ō��v10
	constexpr int kTransformElementNum = 10;
	//�U���̃f�[�^��
	constexpr int kAttackDataElementNum = 13;
	//�X�R�A�f�[�^�̐�
	constexpr int kScoreDataElementNum = 2;
	//Unity�̍��W�Ɋ|���邱�Ƃ�DX���C�u�����ł�Unity�Ɠ����傫���ɂȂ�
	constexpr float kUnityToDXPosition = 100.0f;
}

CSVDataLoader::CSVDataLoader()
{
}

CSVDataLoader::~CSVDataLoader()
{
}

std::vector<ObjectData> CSVDataLoader::LoadTransformDataCSV(const char* fileName)
{
	//�f�[�^���i�[����z��
	std::vector<ObjectData> objects;

	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(fileName, kTransformElementNum);

	for(auto values: valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		ObjectData objData;
		
		//���O
		objData.name = values[0];
		//���W
		objData.pos.x	= std::stof(values[1]) * kUnityToDXPosition;	//std::stof�͕������float�ɕϊ�����
		objData.pos.y	= std::stof(values[2]) * kUnityToDXPosition;
		objData.pos.z	= std::stof(values[3]) * kUnityToDXPosition;
		//��]
		objData.rot.x	= std::stof(values[4]);	
		objData.rot.y	= std::stof(values[5]);
		objData.rot.z	= -1 * std::stof(values[6]);
		//�傫��
		objData.scale.x = std::stof(values[7]);	
		objData.scale.y = std::stof(values[8]);
		objData.scale.z = std::stof(values[9]);

		//�z��ɒǉ�
		objects.emplace_back(objData);
	}

	return objects;
}

std::vector<AttackData> CSVDataLoader::LoadAttackDataCSV(const char* fileName)
{
	//�f�[�^���i�[����z��
	std::vector<AttackData> attackDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(fileName, kAttackDataElementNum);
	for (auto values : valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		AttackData attackData;
		//������̖��O
		attackData.ownerName = values[0];
		//�U���̖��O
		attackData.attackName = values[1];
		//�_���[�W
		attackData.damege = std::stoi(values[2]);
		//����
		attackData.keepFrame = std::stoi(values[3]);
		//����
		attackData.startFrame = std::stoi(values[4]);
		//�m�b�N�o�b�N�̑傫��
		attackData.knockBackPower = std::stof(values[5]);
		//�A�j���[�V����
		attackData.anim =  values[6];
		//�A�j���[�V�������x
		attackData.animSpeed = std::stof(values[7]);
		//�U���̔��a
		attackData.radius = std::stof(values[8]);
		//�K�E�Q�[�W�̏㏸��
		attackData.addUltGage = std::stoi(values[9]);
		//�O�i�t���[��
		attackData.moveFrame = std::stoi(values[10]);
		//�O�i���x
		attackData.moveSpeed = std::stof(values[11]);
		//�U���̏d��
		auto aw = values[12];
		if		(aw == "Light")		attackData.attackWeight	= Battle::AttackWeight::Light;
		else if (aw == "Middle")	attackData.attackWeight	= Battle::AttackWeight::Middle;
		else if (aw == "Heavy")		attackData.attackWeight	= Battle::AttackWeight::Heavy;
		else if (aw == "Heaviest")	attackData.attackWeight = Battle::AttackWeight::Heaviest;
		//�q�b�g�X�g�b�v�̒���
		attackData.hitStopFrame = std::stoi(values[13]);
		//�J�����̗h��
		auto sp = values[14];
		if		(sp == "None")		attackData.shakePower = ShakePower::None;
		else if (sp == "Low")		attackData.shakePower = ShakePower::Low;
		else if (sp == "Middle")	attackData.shakePower = ShakePower::Middle;
		else if (sp == "High")		attackData.shakePower = ShakePower::High;
		else if (sp == "Highest")	attackData.shakePower = ShakePower::Highest;
		//�z��ɒǉ�
		attackDatas.emplace_back(attackData);
	}
	return attackDatas;
}

std::vector<ScoreData> CSVDataLoader::LoadScoreDataCSV(const char* fileName)
{
	//�f�[�^���i�[����z��
	std::vector<ScoreData> scoreDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(fileName, kScoreDataElementNum);
	for (auto values : valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		ScoreData scoreData;
		//������̖��O
		scoreData.dataName = values[0];
		//�U���̖��O
		scoreData.score = std::stoi(values[1]);
		//�z��ɒǉ�
		scoreDatas.emplace_back(scoreData);
	}
	return scoreDatas;
}

//�f�[�^�����ׂēǂݍ���
const std::vector<std::vector<std::string>> CSVDataLoader::GetStringList(const char* fileName, int elementNum)
{
	//�Ԃ��l
	std::vector<std::vector<std::string>> valuesDatas;
	//�t�@�C�����J��
	std::ifstream file(fileName);
	//�������t�@�C�����J���Ȃ�������
	if (!file.is_open())return valuesDatas;//��̃��X�g��Ԃ�
	//1�s���ǂݎ��p�̕ϐ�
	std::string line;
	//�ŏ��̃w�b�_�[�̓X�L�b�v������
	bool isHeader = true;
	//CSV�̏I���܂œǂݎ��
	// getline�œǂݎ���Ă���(�ǂݎ��ʒu�i�����́u�|�C���^�v�j�́A���[�v�̂��тɑO�ɐi�݂܂�)
	//1�s���ǂݎ���Ă����ǂݎ��s���Ȃ��Ȃ�����false�ɂȂ�
	while (std::getline(file, line))//1�s���ǂݎ��
	{
		//�ŏ��̍s�̓X�L�b�v����(�w�b�_�[)
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		//�s���J���}��؂��1���ǂݍ��ނ��߂̏���
		std::stringstream ss(line);			//��������X�g���[��(getline�œǂݎ�邽��)�ɕϊ�
		std::string part;					//�������Ď��o����1�v�f
		std::vector<std::string> values;	//�v�f���܂Ƃ߂��z��
		//�J���}��؂�Ŏ��o���Ă���
		//ss����,��؂�Ŏ��o���Ă���part�ɓ���Ă���
		while (std::getline(ss, part, ',')) {
			values.emplace_back(part);           //�������ꂽ���ڂ����X�g�ɒǉ�
		}
		//�v�f���`�F�b�N
		if (values.size() < elementNum)continue;//�Ȃ��ꍇ�͕s���ȍs�Ȃ̂Ŕ�΂�
		//�f�[�^��z��ɒǉ�
		valuesDatas.emplace_back(values);
	}
	//�Öك��[�u������̂ł����炭���v
	return valuesDatas;
}

#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
//�I�u�W�F�N�g�̏����i�[����\����
struct ObjectData {
    std::string name;      //�I�u�W�F�N�g��
    VECTOR      pos;       //�ʒu
    VECTOR      rot;       //��]�i���W�A���j
    VECTOR      scale;     //�X�P�[��
};
//�I�u�W�F�N�g�̏����i�[����\����
struct AttackData {
    std::string ownerName;      //�L�����N�^�[��
	std::string attackName;		//�U����
	int         damege;		    //�_���[�W
	int         keepFrame;		//����
	int         startFrame;		//����
	float       knockBackPower;	//�m�b�N�o�b�N�̑傫��
	std::string anim;	        //�A�j���[�V����
	float       animSpeed;	    //�A�j���[�V�����̑��x
	float       radius;         //�U���̔��a
	int         addUltGage;     //�U�����q�b�g�����Ƃ��̉��Z�Q�[�W��
	int         moveFrame;      //�O�i����t���[��
    float       moveSpeed;      //�ړ���
};

class CSVDataLoader
{
public:
    CSVDataLoader();
	virtual ~CSVDataLoader();

    /// <summary>
    /// CSV�t�@�C������I�u�W�F�N�g��Tranceform�����擾
    /// </summary>
    /// <param name="fileName">csv</param>
    /// <returns></returns>
    static std::vector<ObjectData> LoadTransformDataCSV(const char* fileName);

    /// <summary>
    /// �U���f�[�^�̓ǂݎ��
    /// </summary>
    /// <param name="fileName"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    static std::vector<AttackData> LoadAttackDataCSV(const char* fileName);

private:
    static const std::vector<std::vector<std::string>> GetStringList(const char* fileName,int elementNum);
};
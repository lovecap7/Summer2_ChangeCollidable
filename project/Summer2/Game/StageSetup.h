#pragma once
#include <vector>
#include <memory>

namespace Stage
{
    enum class StageIndex : int
    {
        Stage1 = 1,
        Stage2 = 2,
        Stage3 = 3,
    };
}

class Player;
class Actor;
/// <summary>
/// �X�e�[�W�̏���������N���X
/// </summary>
class StageSetup
{
public:
    StageSetup(Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageSetup();
    void MovePlayerPointer(std::shared_ptr<Player>& player);
    void MoveActorsPointer(std::vector<std::shared_ptr<Actor>>& actors);
    void End();
private:
    //�X�e�[�W�ԍ�
    Stage::StageIndex m_stageIndex;
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�A�N�^�[
    std::vector<std::shared_ptr<Actor>> m_actors;
    //�n���h��
   //�L�����N�^�[
    int m_playerHandle;
    int m_purpleDinosaurHandle;
    int m_smallDragonHandle;
    int m_bomberHandle;
    int m_bossDragonHandle;
    //�X�e�[�W�̃I�u�W�F�N�g
    int m_wallHandle;
    int m_pathHandle;
    int m_skyHandle;
    //�X�e�[�W�̓����蔻��
    int m_cubeHandle;
    int m_cylinderHandle;
private:
    //�n���h�����[�h
    void LoadHandle();
    //�L�����N�^�[�̍쐬�Ɣz�u
    void CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors);
    //�X�e�[�W�̍쐬�Ɣz�u
    void CreateStage(std::vector<std::shared_ptr<Actor>>& actors);
};


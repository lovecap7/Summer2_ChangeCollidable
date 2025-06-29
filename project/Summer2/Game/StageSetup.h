#pragma once
#include <list>
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
class UIBase;
/// <summary>
/// �X�e�[�W�̏���������N���X
/// </summary>
class StageSetup
{
public:
    StageSetup(Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageSetup();
    //�v���C���[�̃|�C���^���ڂ�
    void MovePlayerPointer(std::shared_ptr<Player>& player);
    //�A�N�^�[���ڂ�
    void MoveActorsPointer(std::list<std::shared_ptr<Actor>>& actors);
    //UI���ڂ�
    void MoveUIPointer(std::list<std::shared_ptr<UIBase>>& uis);

    void End();
private:
    //�X�e�[�W�ԍ�
    Stage::StageIndex m_stageIndex;
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�A�N�^�[
    std::list<std::shared_ptr<Actor>> m_actors;
    //UI
    std::list<std::shared_ptr<UIBase>> m_uis;
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
    //�L�����N�^�[�̍쐬�Ɣz�u�ƕK�v��UI�̍쐬
    void CreateCharacterAndUI();
    //�X�e�[�W�̍쐬�Ɣz�u
    void CreateStage();
};


#pragma once
#include "SceneBase.h"
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

class SceneController;
class Input;
class GameManager;
class StageScene :
    public SceneBase
{
public:
    StageScene(SceneController& controller, Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //�Q�[���}�l�[�W���[
    std::unique_ptr<GameManager> m_gameManager;
    //�X�e�[�W�ԍ�
    Stage::StageIndex m_stageIndex;
};


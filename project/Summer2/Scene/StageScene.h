#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class SceneController;
class Input;
class GameManager;
class StageScene :
    public SceneBase
{
public:
    StageScene(SceneController& controller);
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
    //�Q�[���̊Ǘ�
    std::unique_ptr<GameManager> m_gameManager;
};


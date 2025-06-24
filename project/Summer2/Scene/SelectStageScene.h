#pragma once
#include "SceneBase.h"

class Input;
class SceneController;
class SelectStageScene :
    public SceneBase
{
public:
    SelectStageScene(SceneController& controller);
    ~SelectStageScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
};


#pragma once
#include "SceneBase.h"
class Input;
class SceneController;
class DebugScene :
    public SceneBase
{
public:
    DebugScene(SceneController& controller);
    ~DebugScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //�J�[�\���ړ��̂��߂̃C���f�b�N�X
    int m_selectSceneIndex;
};


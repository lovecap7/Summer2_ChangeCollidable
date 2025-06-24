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
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //カーソル移動のためのインデックス
    int m_selectSceneIndex;
};


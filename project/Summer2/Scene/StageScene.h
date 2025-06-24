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
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
    std::unique_ptr<GameManager> m_gameManager;
};


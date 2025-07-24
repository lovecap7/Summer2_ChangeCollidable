#pragma once
#include "SceneBase.h"
#include <memory>

class SelectStagePlayer;
class SelectStageCamera;
class SceneController;
class SelectStageScene :
    public SceneBase
{
public:
    SelectStageScene(SceneController& controller);
    ~SelectStageScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //カメラ
    std::unique_ptr<SelectStageCamera> m_camera;
    //モデル
    std::unique_ptr<SelectStagePlayer> m_player;
};


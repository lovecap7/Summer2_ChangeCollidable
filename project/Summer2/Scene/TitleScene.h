#pragma once
#include "SceneBase.h"
#include <memory>
class Input;
class TitleCamera;
class SceneController;
class Model;
class TitlePlayer;
class TitleScene :
    public SceneBase
{
public:
    TitleScene(SceneController& controller);
    ~TitleScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
private:
    //カメラ
	std::unique_ptr<TitleCamera> m_camera;
    //モデル
	std::unique_ptr<TitlePlayer> m_player;
};


#pragma once
#include "SceneBase.h"
class SceneController;
class GameoverScene :
    public SceneBase
{
public:
    GameoverScene(SceneController& controller);
    ~GameoverScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //状態遷移
    using UpdateFunc_t = void(GameoverScene::*)();
    using DrawFunc_t = void(GameoverScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //出現中状態
    void AppearUpdate();
    //ノーマル状態
    void NormalUpdate();
    //消滅中状態
    void DisappearUpdate();
    //通常描画
    void NormalDraw();
private:
    int m_countFrame;
};


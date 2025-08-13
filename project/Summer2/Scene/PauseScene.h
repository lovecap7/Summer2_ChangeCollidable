#pragma once
#include "SceneBase.h"
class SceneController;
class PauseScene :
    public SceneBase
{
public:
    PauseScene(SceneController& controller);
    ~PauseScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //状態遷移
    using UpdateFunc_t = void(PauseScene::*)();
    using DrawFunc_t = void(PauseScene::*)();
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
    //途中描画
    void ShiftingDraw();
    //ゲームに戻る
    void RetrunGame();
private:
    int m_countFrame;
    //メニューセレクト
    int m_menuSelectIndex;
};


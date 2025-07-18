#pragma once
#include "SceneBase.h"
class SceneController;
class PauseScene :
    public SceneBase
{
public:
    PauseScene(SceneController& controller);
    ~PauseScene();
    //”h¶ƒNƒ‰ƒX‚ÅÀ‘•‚ğÀ‘•
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //ó‘Ô‘JˆÚ
    using UpdateFunc_t = void(PauseScene::*)();
    using DrawFunc_t = void(PauseScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //oŒ»’†ó‘Ô
    void AppearUpdate();
    //ƒm[ƒ}ƒ‹ó‘Ô
    void NormalUpdate();
    //Á–Å’†ó‘Ô
    void DisappearUpdate();
    //’Êí•`‰æ
    void NormalDraw();
    //“r’†•`‰æ
    void ShiftingDraw();
private:
    int m_countFrame;
};


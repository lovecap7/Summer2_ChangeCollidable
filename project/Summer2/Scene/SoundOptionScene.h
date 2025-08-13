#pragma once
#include "SceneBase.h"
class SceneController;
class SoundOptionScene :
    public SceneBase
{
public:
    SoundOptionScene(SceneController& controller);
    ~SoundOptionScene();
    //”h¶ƒNƒ‰ƒX‚ÅÀ‘•‚ğÀ‘•
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
};


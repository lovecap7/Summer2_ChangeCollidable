#pragma once
#include "SceneBase.h"

class Input;
class SceneController;
class ResultScene :
    public SceneBase
{
public:
    ResultScene(SceneController& controller);
    ~ResultScene();
    //”h¶ƒNƒ‰ƒX‚ÅÀ‘•‚ğÀ‘•
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
    
};


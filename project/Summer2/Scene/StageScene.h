#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

namespace Stage
{
    enum class StageIndex : int
    {
        Stage1 = 1,
        Stage2 = 2,
        Stage3 = 3,
    };
}

class SceneController;
class Input;
class GameManager;
class StageScene :
    public SceneBase
{
public:
    StageScene(SceneController& controller, Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //ゲームマネージャー
    std::unique_ptr<GameManager> m_gameManager;
    //ステージ番号
    Stage::StageIndex m_stageIndex;
};


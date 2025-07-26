#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
class SceneController;
class Score;
class GameClearScene :
    public SceneBase
{
public:
    GameClearScene(SceneController& controller,std::shared_ptr<Score> score, Stage::StageIndex index);
    ~GameClearScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //状態遷移
    using UpdateFunc_t = void(GameClearScene::*)();
    using DrawFunc_t = void(GameClearScene::*)();
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
    std::shared_ptr<Score> m_score;
    //ステージの番号
    Stage::StageIndex m_stageIndex;
  
};


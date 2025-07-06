#pragma once
#include <vector>
#include <memory>
#include "../Scene/StageScene.h"

class Camera;
class ActorManager;
class UIManager;
class Score;
class Timer;
class GameManager
{
public:
    GameManager(Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~GameManager();
    void Init();
    void Update();
    void Draw()const;
    void End();
    //ゲームオーバー
    bool IsGameover() { return m_isGameover; };
    //ゲームクリア
    bool IsGameClear() { return m_isGameClear; };
    //スコア
    std::shared_ptr<Score> GetScore() { return m_score; };
private:
	//アクターマネージャー
	std::shared_ptr<ActorManager> m_actorManager;
    //UIマネージャー
	std::shared_ptr<UIManager> m_uiManager;
    //カメラ
    std::shared_ptr<Camera> m_camera;
    //スコア
    std::shared_ptr<Score> m_score;
    //タイマー
    std::shared_ptr<Timer> m_timer;
private:
    //ゲームオーバーフラグ
    bool m_isGameover;
    //ゲームクリアフラグ
    bool m_isGameClear;
    //デバッグモード用
    bool m_isUpdateStop = false;
};

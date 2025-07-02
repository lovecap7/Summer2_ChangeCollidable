#pragma once
#include <vector>
#include <memory>

class Camera;
class ActorManager;
class UIManager;
class StageSetup;
class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Init();
    void Update();
    void Draw()const;
    void End();
    //ゲームオーバー
    bool IsGameover() { return m_isGameover; };
    //ゲームクリア
    bool IsGameClear() { return m_isGameClear; };
private:
	//アクターマネージャー
	std::shared_ptr<ActorManager> m_actorManager;
    //UIマネージャー
	std::shared_ptr<UIManager> m_uiManager;
    //カメラ
    std::shared_ptr<Camera> m_camera;
    //ステージセットアップ
    std::unique_ptr<StageSetup> m_stageSetup;
private:
    //ゲームオーバーフラグ
    bool m_isGameover;
    //ゲームクリアフラグ
    bool m_isGameClear;
    //デバッグモード用
    bool m_isUpdateStop = false;
};

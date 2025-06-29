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
    //デバッグモード用
    bool m_isUpdateStop = false;
};

#pragma once
#include <vector>
#include <memory>

class Player;
class Camera;
class SceneController;
class Input;
class Actor;
class ActorManager;
class UIManager;
class StageSetup;
class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Init();
    void Update(Input& input);
    void Draw()const;
    void End();
private:
    //プレイヤー
    std::shared_ptr<Player> m_player;
    //アクターマネージャー
    std::shared_ptr<ActorManager> m_actorManager;
    //カメラ
    std::unique_ptr<Camera> m_camera;
    //UI
    std::shared_ptr<UIManager> m_uiManager;
    //ステージセットアップ
    std::shared_ptr<StageSetup> m_stageSetup;
private:
    //デバッグモード用
    bool m_isUpdateStop = false;
};

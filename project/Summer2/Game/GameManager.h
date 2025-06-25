#pragma once
#include <vector>
#include <memory>

class Player;
class Camera;
class SceneController;
class Input;
class Actor;
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
    //アクター
	std::vector<std::shared_ptr<Actor>> m_actors;
    //プレイヤー
    std::shared_ptr<Player> m_player;
    //カメラ
    std::shared_ptr<Camera> m_camera;
    //ステージセットアップ
    std::shared_ptr<StageSetup> m_stageSetup;
private:
    //デバッグモード用
    bool m_isUpdateStop = false;
};

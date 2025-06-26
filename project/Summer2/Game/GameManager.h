#pragma once
#include <vector>
#include <memory>

class Camera;
class ActorManager;
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
    //カメラ
    std::shared_ptr<Camera> m_camera;
private:
    //デバッグモード用
    bool m_isUpdateStop = false;
};

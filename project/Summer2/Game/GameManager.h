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
	//�A�N�^�[�}�l�[�W���[
	std::shared_ptr<ActorManager> m_actorManager;
    //�J����
    std::shared_ptr<Camera> m_camera;
private:
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

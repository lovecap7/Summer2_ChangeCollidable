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
	//�A�N�^�[�}�l�[�W���[
	std::shared_ptr<ActorManager> m_actorManager;
    //UI�}�l�[�W���[
	std::shared_ptr<UIManager> m_uiManager;
    //�J����
    std::shared_ptr<Camera> m_camera;
    //�X�e�[�W�Z�b�g�A�b�v
    std::unique_ptr<StageSetup> m_stageSetup;
private:
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

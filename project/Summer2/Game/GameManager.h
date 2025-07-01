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
    //�Q�[���I�[�o�[
    bool IsGameover() { return m_isGameover; };
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
    //�Q�[���I�[�o�[�t���O
    bool m_isGameover;
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

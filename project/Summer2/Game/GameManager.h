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
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�A�N�^�[�}�l�[�W���[
    std::shared_ptr<ActorManager> m_actorManager;
    //�J����
    std::unique_ptr<Camera> m_camera;
    //UI
    std::shared_ptr<UIManager> m_uiManager;
    //�X�e�[�W�Z�b�g�A�b�v
    std::shared_ptr<StageSetup> m_stageSetup;
private:
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

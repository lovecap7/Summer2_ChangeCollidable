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
    //�A�N�^�[
	std::vector<std::shared_ptr<Actor>> m_actors;
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�J����
    std::shared_ptr<Camera> m_camera;
    //�X�e�[�W�Z�b�g�A�b�v
    std::shared_ptr<StageSetup> m_stageSetup;
private:
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

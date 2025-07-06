#pragma once
#include <vector>
#include <memory>
#include "../Scene/StageScene.h"

class Camera;
class ActorManager;
class UIManager;
class Score;
class Timer;
class GameManager
{
public:
    GameManager(Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~GameManager();
    void Init();
    void Update();
    void Draw()const;
    void End();
    //�Q�[���I�[�o�[
    bool IsGameover() { return m_isGameover; };
    //�Q�[���N���A
    bool IsGameClear() { return m_isGameClear; };
    //�X�R�A
    std::shared_ptr<Score> GetScore() { return m_score; };
private:
	//�A�N�^�[�}�l�[�W���[
	std::shared_ptr<ActorManager> m_actorManager;
    //UI�}�l�[�W���[
	std::shared_ptr<UIManager> m_uiManager;
    //�J����
    std::shared_ptr<Camera> m_camera;
    //�X�R�A
    std::shared_ptr<Score> m_score;
    //�^�C�}�[
    std::shared_ptr<Timer> m_timer;
private:
    //�Q�[���I�[�o�[�t���O
    bool m_isGameover;
    //�Q�[���N���A�t���O
    bool m_isGameClear;
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
};

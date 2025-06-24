#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>
class ActorManager;
class Player;
class Camera;
class SceneController;
class Input;
class TestCollScene :
    public SceneBase
{
public:
    TestCollScene(SceneController& controller);
    ~TestCollScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //�A�N�^�[�}�l�[�W���[
    std::shared_ptr<ActorManager> m_actorManager;
    //�v���C���[
    std::shared_ptr<Player> m_player;
    //�J����
    std::unique_ptr<Camera> m_camera;
private:
    //�n���h��
    int m_playerHandle;
    int m_polygonHandle;
    int m_wallHandle;
    int m_purpleDinosaurHandle;
    int m_smallDragonHandle;
    int m_bossDragonHandle;
private:
#if _DEBUG
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
#endif
};


#pragma once
#include "SceneBase.h"
class SceneController;
class GameClearScene :
    public SceneBase
{
public:
    GameClearScene(SceneController& controller);
    ~GameClearScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
private:
    //��ԑJ��
    using UpdateFunc_t = void(GameClearScene::*)();
    using DrawFunc_t = void(GameClearScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //�o�������
    void AppearUpdate();
    //�m�[�}�����
    void NormalUpdate();
    //���Œ����
    void DisappearUpdate();
    //�ʏ�`��
    void NormalDraw();
private:
    int m_countFrame;
};


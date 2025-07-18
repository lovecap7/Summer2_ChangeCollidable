#pragma once
#include "SceneBase.h"
class SceneController;
class PauseScene :
    public SceneBase
{
public:
    PauseScene(SceneController& controller);
    ~PauseScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //��ԑJ��
    using UpdateFunc_t = void(PauseScene::*)();
    using DrawFunc_t = void(PauseScene::*)();
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
    //�r���`��
    void ShiftingDraw();
private:
    int m_countFrame;
};


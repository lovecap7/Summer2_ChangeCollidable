#pragma once
#include "SceneBase.h"
class SceneController;
class GameoverScene :
    public SceneBase
{
public:
    GameoverScene(SceneController& controller);
    ~GameoverScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //��ԑJ��
    using UpdateFunc_t = void(GameoverScene::*)();
    using DrawFunc_t = void(GameoverScene::*)();
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


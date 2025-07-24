#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
class Input;
class TitleCamera;
class SceneController;
class Model;
class TitlePlayer;
class Actor;
class TitleScene :
    public SceneBase
{
public:
    TitleScene(SceneController& controller);
    ~TitleScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
    //ハンドルロード
    void LoadHandle();
    //配置
    void LoadStage();
    //ハンドル削除
    void AllDeleteHandle();
    //オブジェ削除
    void AllDeleteStage();
private:
    //カメラ
	std::unique_ptr<TitleCamera> m_camera;
    //プレイヤー
	std::shared_ptr<TitlePlayer> m_player;
    //描画用オブジェ
    std::vector<std::shared_ptr<Actor>> m_stageObjects;
    //ハンドル
    std::map<std::string, int> m_stageHandles;
    //影
    int m_shadowMapHandle;
    //ライトハンドル
    int m_lightHandle;
private:
    //ライトの初期化
    void InitLight();
    //シャドウマップの初期化
    void InitShadow();
    //シャドウの更新
    void UpdateShadow();
};


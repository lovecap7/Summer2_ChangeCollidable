#pragma once
#include <list>
#include <memory>

namespace Stage
{
    enum class StageIndex : int
    {
        Stage1 = 1,
        Stage2 = 2,
        Stage3 = 3,
    };
}

class Player;
class Actor;
class UIBase;
/// <summary>
/// ステージの準備をするクラス
/// </summary>
class StageSetup
{
public:
    StageSetup(Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageSetup();
    //アクターを移す
    void MoveActorsPointer(std::list<std::shared_ptr<Actor>>& actors);
    //UIを移す
    void MoveUIPointer(std::list<std::shared_ptr<UIBase>>& uis);
    //終了処理
    void End();
private:
    //ステージ番号
    Stage::StageIndex m_stageIndex;
    //アクター
    std::list<std::shared_ptr<Actor>> m_actors;
    //UI
    std::list<std::shared_ptr<UIBase>> m_uis;
    //ハンドル
   //キャラクター
    int m_playerHandle;
    int m_purpleDinosaurHandle;
    int m_smallDragonHandle;
    int m_bomberHandle;
    int m_bossDragonHandle;
    //ステージのオブジェクト
    int m_wallHandle;
    int m_pathHandle;
    int m_skyHandle;
    int m_blockGrassHandle; //草の地面
    //ステージの当たり判定
    int m_cubeHandle;
    int m_cylinderHandle;
    int m_planeHandle;
private:
    //ハンドルロード
    void LoadHandle();
    //キャラクターの作成と配置と必要なUIの作成
    void CreateCharacterAndUI();
    //ステージの作成と配置
    void CreateStage();
    //ボス部屋
    void CreateBossArea();
};


#pragma once
#include <vector>
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
/// <summary>
/// ステージの準備をするクラス
/// </summary>
class StageSetup
{
public:
    StageSetup(Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageSetup();
    void MovePlayerPointer(std::shared_ptr<Player>& player);
    void MoveActorsPointer(std::vector<std::shared_ptr<Actor>>& actors);
    void End();
private:
    //ステージ番号
    Stage::StageIndex m_stageIndex;
    //プレイヤー
    std::shared_ptr<Player> m_player;
    //アクター
    std::vector<std::shared_ptr<Actor>> m_actors;
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
    //ステージの当たり判定
    int m_cubeHandle;
    int m_cylinderHandle;
private:
    //ハンドルロード
    void LoadHandle();
    //キャラクターの作成と配置
    void CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors);
    //ステージの作成と配置
    void CreateStage(std::vector<std::shared_ptr<Actor>>& actors);
};


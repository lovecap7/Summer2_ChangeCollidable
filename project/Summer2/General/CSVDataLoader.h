#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
//オブジェクトの情報を格納する構造体
struct ObjectData {
    std::string name;      //オブジェクト名
    VECTOR      pos;       //位置
    VECTOR      rot;       //回転（ラジアン）
    VECTOR      scale;     //スケール
};
//オブジェクトの情報を格納する構造体
struct AttackData {
    std::string ownerName;      //キャラクター名
	std::string attackName;		//攻撃名
	int         damege;		    //ダメージ
	int         keepFrame;		//持続
	int         startFrame;		//発生
	float       knockBackPower;	//ノックバックの大きさ
	std::string anim;	        //アニメーション
	float       animSpeed;	    //アニメーションの速度
	float       radius;         //攻撃の半径
	int         addUltGage;     //攻撃がヒットしたときの加算ゲージ量
	int         moveFrame;      //前進するフレーム
    float       moveSpeed;      //移動量
};

class CSVDataLoader
{
public:
    CSVDataLoader();
	virtual ~CSVDataLoader();

    /// <summary>
    /// CSVファイルからオブジェクトのTranceform情報を取得
    /// </summary>
    /// <param name="fileName">csv</param>
    /// <returns></returns>
    static std::vector<ObjectData> LoadTransformDataCSV(const char* fileName);

    /// <summary>
    /// 攻撃データの読み取り
    /// </summary>
    /// <param name="fileName"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    static std::vector<AttackData> LoadAttackDataCSV(const char* fileName);

private:
    static const std::vector<std::vector<std::string>> GetStringList(const char* fileName,int elementNum);
};
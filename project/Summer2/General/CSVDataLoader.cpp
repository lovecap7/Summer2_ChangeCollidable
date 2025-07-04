#include "CSVDataLoader.h"
#include <fstream>   // ファイル読み込み用
#include <sstream>   // 文字列分解用（stringstream）stringをファイルのように扱える

namespace
{
	//名前、座標XYZ、回転XYZ、大きさXYZ　で合計10
	constexpr int kTransformElementNum = 10;
	//Unityの座標に掛けることでDXライブラリでもUnityと同じ大きさになる
	constexpr float kUnityToDXPosition = 100.0f;
}

CSVDataLoader::CSVDataLoader()
{
}

CSVDataLoader::~CSVDataLoader()
{
}

std::vector<ObjectData> CSVDataLoader::LoadTransformDataCSV(const char* fileName)
{
	//データを格納する配列
	std::vector<ObjectData> objects;

	//データをすべて読み込む
	auto valuesDatas = GetStringList(fileName, kTransformElementNum);

	for(auto values: valuesDatas)
	{
		//構造体にデータを入れていく
		ObjectData objData;
		
		//名前
		objData.name = values[0];
		//座標
		objData.pos.x	= std::stof(values[1]) * kUnityToDXPosition;	//std::stofは文字列をfloatに変換する
		objData.pos.y	= std::stof(values[2]) * kUnityToDXPosition;
		objData.pos.z	= std::stof(values[3]) * kUnityToDXPosition;
		//回転
		objData.rot.x	= std::stof(values[4]);	
		objData.rot.y	= std::stof(values[5]);
		objData.rot.z	= -1 * std::stof(values[6]);
		//大きさ
		objData.scale.x = std::stof(values[7]);	
		objData.scale.y = std::stof(values[8]);
		objData.scale.z = std::stof(values[9]);

		//配列に追加
		objects.emplace_back(objData);
	}

	return objects;
}

std::vector<AttackData> CSVDataLoader::LoadAttackDataCSV(const char* fileName)
{
	
	//データを格納する配列
	std::vector<AttackData> attackDatas;

	//データをすべて読み込む
	auto valuesDatas = GetStringList(fileName, 12);

	for (auto values : valuesDatas)
	{
		//構造体にデータを入れていく
		AttackData attackData;

		//持ち主の名前
		attackData.ownerName = values[0];
		//攻撃の名前
		attackData.attackName = values[1];
		//ダメージ
		attackData.damege = std::stoi(values[2]);
		//持続
		attackData.keepFrame = std::stoi(values[3]);
		//発生
		attackData.startFrame = std::stoi(values[4]);
		//ノックバックの大きさ
		attackData.knockBackPower = std::stof(values[5]);
		//アニメーション
		attackData.anim =  values[6];
		//アニメーション速度
		attackData.animSpeed = std::stof(values[7]);
		//攻撃の半径
		attackData.radius = std::stof(values[8]);
		//必殺ゲージの上昇量
		attackData.addUltGage = std::stoi(values[9]);
		//前進フレーム
		attackData.moveFrame = std::stoi(values[10]);
		//前進速度
		attackData.moveSpeed = std::stof(values[11]);

		//配列に追加
		attackDatas.emplace_back(attackData);
	}

	return attackDatas;
}

//データをすべて読み込む
const std::vector<std::vector<std::string>> CSVDataLoader::GetStringList(const char* fileName, int elementNum)
{
	//返す値
	std::vector<std::vector<std::string>> valuesDatas;

	//ファイルを開く
	std::ifstream file(fileName);
	//もしもファイルを開けなかったら
	if (!file.is_open())return valuesDatas;//空のリストを返す

	//1行ずつ読み取る用の変数
	std::string line;
	//最初のヘッダーはスキップしたい
	bool isHeader = true;

	//CSVの終わりまで読み取る
	// getlineで読み取っていく(読み取り位置（内部の「ポインタ」）は、ループのたびに前に進みます)
	//1行ずつ読み取っていき読み取る行がなくなったらfalseになる
	while (std::getline(file, line))//1行ずつ読み取る
	{
		//最初の行はスキップする(ヘッダー)
		if (isHeader)
		{
			isHeader = false;
			continue;
		}

		//行をカンマ区切りで1つずつ読み込むための準備
		std::stringstream ss(line);			//文字列をストリーム(getlineで読み取るため)に変換
		std::string part;					//分解して取り出した1要素
		std::vector<std::string> values;	//要素をまとめた配列

		//カンマ区切りで取り出していく
		//ssから,区切りで取り出していきpartに入れていく
		while (std::getline(ss, part, ',')) {
			values.emplace_back(part);           //分割された項目をリストに追加
		}

		//要素数チェック
		if (values.size() < elementNum)continue;//ない場合は不正な行なので飛ばす
		
		//データを配列に追加
		valuesDatas.emplace_back(values);
	}

	//暗黙ムーブが走るのでおそらく大丈夫
	return valuesDatas;
}

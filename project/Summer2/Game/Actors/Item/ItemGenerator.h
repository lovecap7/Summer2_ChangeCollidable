#pragma once
#include <memory>
#include <vector>
#include "ItemBase.h"
#include "../../../General/Math/Vector3.h"
class ActorManager;
class ItemGenerator
{
public:
	ItemGenerator();
	~ItemGenerator();
	void End();
	//アイテムをランダムに生成
	void RandGenerateItem(Vector3 pos);
	//アイテムをまとめた配列を渡す
	void MoveItems(std::shared_ptr<ActorManager> actorManager);
	//回復の生成
	void GenerateHeart(Vector3 pos);
	//爆弾の生成
	void GenerateBomb(Vector3 pos, Vector3 vec);
	//必殺ゲージアップの生成
	void GenerateUltGageUp(Vector3 pos);
	//防御力アップの生成
	void GenerateDefenseUp(Vector3 pos);
private:
	//アイテムをまとめた配列
	std::vector<std::shared_ptr<ItemBase>> m_items;
	//ハンドル
	int m_heartHandle;			//回復
	int m_bombHandle;			//爆弾
	int m_ultGageUpHandle;		//必殺ゲージアップ
	int m_defenseHandle;		//耐久アップ
};


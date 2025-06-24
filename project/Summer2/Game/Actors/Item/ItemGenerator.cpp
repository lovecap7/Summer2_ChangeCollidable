#include "ItemGenerator.h"
#include "../ActorManager.h"
#include "../Actor.h"
#include "Heart.h"
#include "Bomb.h"
#include "UltGageUp.h"
#include "DefenseUp.h"
#include <DxLib.h>
#include <cassert>

ItemGenerator::ItemGenerator() :
	m_heartHandle(MV1LoadModel("Data/Model/Item/Heart.mv1")),
	m_bombHandle(MV1LoadModel("Data/Model/Item/Bomb.mv1")),
	m_ultGageUpHandle(MV1LoadModel("Data/Model/Item/UltGageUp.mv1")),
	m_defenseHandle(MV1LoadModel("Data/Model/Item/DefenseUp.mv1"))
{
	assert(m_heartHandle >= 0);
}

ItemGenerator::~ItemGenerator()
{
}

void ItemGenerator::End()
{
	MV1DeleteModel(m_heartHandle);
	MV1DeleteModel(m_bombHandle);
	MV1DeleteModel(m_ultGageUpHandle);
}

void ItemGenerator::RandGenerateItem(Vector3 pos)
{
	//ランダムに決定
	auto rand = 4;//GetRand(1);
	switch (rand)
	{
	case static_cast<int>(ItemKind::Heart):
		GenerateHeart(pos);
		break;
	case static_cast<int>(ItemKind::Bomb):
		GenerateBomb(pos, Vector3::Zero());
		break;
	case static_cast<int>(ItemKind::UltGageUp):
		GenerateUltGageUp(pos);
		break;
	case static_cast<int>(ItemKind::AttackUp):
		GenerateUltGageUp(pos);
		break;
	case static_cast<int>(ItemKind::DefenseUp):
		GenerateDefenseUp(pos);
		break;
	default:
		break;
	}
}

void ItemGenerator::MoveItems(std::shared_ptr<ActorManager> actorManager)
{
	if (m_items.empty())return;//空なら渡すものはない
	for (auto& item : m_items)
	{
		actorManager->SetNewActor(item);//アクターに追加
	}
	m_items.clear();//削除
}

void ItemGenerator::GenerateHeart(Vector3 pos)
{
	std::shared_ptr<ItemBase> item = nullptr;
	item = std::make_shared<Heart>(MV1DuplicateModel(m_heartHandle), pos);
	m_items.emplace_back(item);
}

void ItemGenerator::GenerateBomb(Vector3 pos, Vector3 vec)
{
	std::shared_ptr<ItemBase> item = nullptr;
	item = std::make_shared<Bomb>(MV1DuplicateModel(m_bombHandle), pos, vec);
	m_items.emplace_back(item);
}

void ItemGenerator::GenerateUltGageUp(Vector3 pos)
{
	std::shared_ptr<ItemBase> item = nullptr;
	item = std::make_shared<UltGageUp>(MV1DuplicateModel(m_ultGageUpHandle), pos);
	m_items.emplace_back(item);
}

void ItemGenerator::GenerateDefenseUp(Vector3 pos)
{
	std::shared_ptr<ItemBase> item = nullptr;
	item = std::make_shared<DefenseUp>(MV1DuplicateModel(m_defenseHandle), pos);
	m_items.emplace_back(item);
}

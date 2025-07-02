#include "StageSetup.h"
#include "../General/Model.h"
#include <cassert>
#include <string>
#include <DxLib.h>
//配置データ
#include "../General/TransformDataLoader.h"
//アクター
#include "../Game/Actors/Actor.h"
//プレイヤー
#include "../Game/Actors/Character/Player/Player.h"
//敵
#include "../Game/Actors/Character/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "../Game/Actors/Character/Enemy/SmallDragon/SmallDragon.h"
#include "../Game/Actors/Character/Enemy/BossDragon/BossDragon.h"
#include "../Game/Actors/Character/Enemy/Bomber/Bomber.h"
#include "../Game/Actors/Character/Enemy/EnemyBase.h"
//ステージ
#include "../Game/Actors/Stage/InvisibleWall.h"
#include "../Game/Actors/Stage/StageObjectCollision.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../Game/Actors/Stage/Sky.h"
#include "../Game/Actors/Stage/BossArea.h"
//UI
#include "../Game/UI/PlayerUI/PlayerHPUI.h"
#include "../Game/UI/PlayerUI/PlayerUltGageUI.h"
#include "../Game/UI/EnemyUI/EnemyHPUI.h"
#include "../Game/UI/EnemyUI/BossHPUI.h"

StageSetup::StageSetup(Stage::StageIndex index):
	m_stageIndex(index)
{
	//ハンドルの準備
	LoadHandle();
	//キャラクターの作成
	CreateCharacterAndUI();
	//ステージのオブジェクト配置
	CreateStage();
	//ボス部屋を作る
	CreateBossArea();
}

StageSetup::~StageSetup()
{
}

void StageSetup::MoveActorsPointer(std::list<std::shared_ptr<Actor>>& actors)
{
	//アクターを渡す
	actors = std::move(m_actors);
	//メモリを解放
	m_actors.clear();
}

void StageSetup::MoveUIPointer(std::list<std::shared_ptr<UIBase>>& uis)
{
	//UIを渡す
	uis = std::move(m_uis);
	m_uis.clear();
}

void StageSetup::End()
{
	MV1DeleteModel(m_playerHandle);
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		MV1DeleteModel(m_wallHandle);
		MV1DeleteModel(m_purpleDinosaurHandle);
		MV1DeleteModel(m_smallDragonHandle);
		MV1DeleteModel(m_bossDragonHandle);
		MV1DeleteModel(m_bomberHandle);
		MV1DeleteModel(m_pathHandle);
		MV1DeleteModel(m_cubeHandle);
		MV1DeleteModel(m_cylinderHandle);
		MV1DeleteModel(m_skyHandle);
		MV1DeleteModel(m_planeHandle);
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	};
}

void StageSetup::LoadHandle()
{
	m_playerHandle = MV1LoadModel("Data/Model/Player/Player.mv1");
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		m_wallHandle = MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1");
		m_purpleDinosaurHandle = MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1");
		m_smallDragonHandle = MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1");
		m_bomberHandle = MV1LoadModel("Data/Model/Enemy/Bomber.mv1");
		m_bossDragonHandle = MV1LoadModel("Data/Model/Enemy/BossDragon.mv1");
		m_pathHandle = MV1LoadModel("Data/Model/Stage/1/Path.mv1");
		m_cubeHandle = MV1LoadModel("Data/Model/Collision/Cube.mv1");
		m_cylinderHandle = MV1LoadModel("Data/Model/Collision/Cylinder.mv1");
		m_planeHandle = MV1LoadModel("Data/Model/Collision/Plane.mv1");
		m_skyHandle = MV1LoadModel("Data/Model/Stage/Sky/Sky_Daylight02.pmx");
		assert(m_playerHandle >= 0);
		assert(m_wallHandle >= 0);
		assert(m_purpleDinosaurHandle >= 0);
		assert(m_smallDragonHandle >= 0);
		assert(m_bossDragonHandle >= 0);
		assert(m_pathHandle >= 0);
		assert(m_cubeHandle >= 0);
		assert(m_cylinderHandle >= 0);
		assert(m_skyHandle >= 0);
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	};
}

void StageSetup::CreateCharacterAndUI()
{
	//配置データを取得
	std::string path;
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		path = "Data/CSV/CharacterTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	auto characterData = TransformDataLoader::LoadDataCSV(path.c_str());
	//名前からオブジェクトを配置していく
	for (auto& charaData : characterData)
	{
		if (charaData.name == "Player")
		{
			//プレイヤー作成
			std::shared_ptr<Player> player = std::make_shared<Player>(m_playerHandle, charaData.pos);
			player->GetModel()->SetScale(charaData.scale);
			player->GetModel()->SetRot(charaData.rot);
			m_actors.emplace_back(player);
			//プレイヤーの体力UI
			m_uis.emplace_back(std::make_shared<PlayerHPUI>(player));
			//プレイヤーのゲージUI
			m_uis.emplace_back(std::make_shared<PlayerUltGageUI>(player));
		}
		else if (charaData.name == "SmallDragon")
		{
			std::shared_ptr<EnemyBase> smallDragon = std::make_shared<SmallDragon>(MV1DuplicateModel(m_smallDragonHandle),
				charaData.pos);
			smallDragon->GetModel()->SetScale(charaData.scale);
			smallDragon->GetModel()->SetRot(charaData.rot);
			m_actors.emplace_back(smallDragon);
			//敵の体力UI
			m_uis.emplace_back(std::make_shared<EnemyHPUI>(smallDragon));
		}
		else if (charaData.name == "BossDragon")
		{
			std::shared_ptr<EnemyBase> bossDragon = std::make_shared<BossDragon>(MV1DuplicateModel(m_bossDragonHandle),
				charaData.pos);
			bossDragon->GetModel()->SetScale(charaData.scale);
			bossDragon->GetModel()->SetRot(charaData.rot);
			m_actors.emplace_back(bossDragon);
			//ボスのHP
			m_uis.emplace_back(std::make_shared<BossHPUI>(bossDragon));

		}
		else if (charaData.name == "Bomber")
		{
			std::shared_ptr<EnemyBase> bomber = std::make_shared<Bomber>(MV1DuplicateModel(m_bomberHandle),
				charaData.pos);
			bomber->GetModel()->SetScale(charaData.scale);
			bomber->GetModel()->SetRot(charaData.rot);
			m_actors.emplace_back(bomber);
			//敵の体力UI
			m_uis.emplace_back(std::make_shared<EnemyHPUI>(bomber));
		}
		else if (charaData.name == "PurpleDinosaur")
		{
			std::shared_ptr<EnemyBase> purpleDinosaur = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_purpleDinosaurHandle),
				charaData.pos);
			purpleDinosaur->GetModel()->SetScale(charaData.scale);
			purpleDinosaur->GetModel()->SetRot(charaData.rot);
			m_actors.emplace_back(purpleDinosaur);
			//敵の体力UI
			m_uis.emplace_back(std::make_shared<EnemyHPUI>(purpleDinosaur));
		}
	}
}

void StageSetup::CreateStage()
{
	//空を作成
	m_actors.emplace_back(std::make_shared<Sky>(m_skyHandle));
	//配置データを取得
	std::string drawPath;
	std::string collPath;
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		drawPath = "Data/CSV/StageTransformData.csv";
		collPath = "Data/CSV/StageCollisionTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	//描画用
	//配置データを取得
	auto stageDrawData = TransformDataLoader::LoadDataCSV(drawPath.c_str());
	//名前からオブジェクトを配置していく
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> path =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_pathHandle), stageData.pos, stageData.scale, stageData.rot);
			m_actors.emplace_back(path);
		}
		else if (stageData.name == "Plane")
		{
			std::shared_ptr<StageObjectDraw> plane =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_actors.emplace_back(plane);
		}
	}
	//当たり判定用
	//配置データを取得
	auto stageCollData = TransformDataLoader::LoadDataCSV(collPath.c_str());
	//名前からコリジョンを配置していく
	for (auto& stageData : stageCollData)
	{
		if (stageData.name == "Plane")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_actors.emplace_back(plane);
		}
		else if (stageData.name == "BossAreaWall")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_actors.emplace_back(plane);
		}
	}
}

void StageSetup::CreateBossArea()
{
	//配置データを取得
	std::string path;
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		path = "Data/CSV/BossTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	//ボス部屋を作成
	auto stageCollData = TransformDataLoader::LoadDataCSV(path.c_str());
	VECTOR startPos = {};
	VECTOR endPos = {};
	//名前からコリジョンを配置していく
	for (auto& stageData : stageCollData)
	{
		if (stageData.name == "Start")
		{
			startPos = stageData.pos;
		}
		else if (stageData.name == "End")
		{
			endPos = stageData.pos;
		}
	}
	m_actors.emplace_back(std::make_shared<BossArea>(startPos, endPos));
}

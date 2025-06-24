#include "StageSetup.h"
#include "../General/TransformDataLoader.h"
#include "../General/Model.h"
#include "Actors/Actor.h"
#include <cassert>
#include <string>
#include <DxLib.h>
//�z�u�f�[�^
#include "../General/TransformDataLoader.h"
//�A�N�^�[
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/ActorManager.h"
//�v���C���[
#include "../Game/Actors/Player/Player.h"
//�G
#include "../Game/Actors/Enemy/EnemyManager.h"
#include "../Game/Actors/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "../Game/Actors/Enemy/SmallDragon/SmallDragon.h"
#include "../Game/Actors/Enemy/Bomber/Bomber.h"
#include "../Game/Actors/Enemy/BossDragon/BossDragon.h"
#include "../Game/Actors/Enemy/EnemyBase.h"
//�X�e�[�W
#include "../Game/Actors/Stage/InvisibleWall.h"
#include "../Game/Actors/Stage/StageObjectCollision.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../Game/Actors/Stage/Sky.h"

StageSetup::StageSetup(Stage::StageIndex index):
	m_stageIndex(index)
{
	//�n���h���̏���
	LoadHandle();
	//�L�����N�^�[�̍쐬
	CreateCharacter(m_actors);
	//�X�e�[�W�̃I�u�W�F�N�g�z�u
	CreateStage(m_actors);

	auto floor = std::make_shared<InvisibleWall>(m_wallHandle, Vector3{ 0.0f,-10.0f,0.0f }, VGet(1000.0f, 1.0f, 1000.0f), VGet(0.0f, 0.0f, 0.0f));
	m_actors.emplace_back(floor);

}

StageSetup::~StageSetup()
{
}

void StageSetup::MovePlayerPointer(std::shared_ptr<Player>& player)
{
	//�v���C���[��n��
	player = std::move(m_player);
}

void StageSetup::MoveActorsPointer(std::vector<std::shared_ptr<Actor>>& actors)
{
	//�A�N�^�[��n��
	actors = std::move(m_actors);
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
		MV1DeleteModel(m_pathHandle);
		MV1DeleteModel(m_cubeHandle);
		MV1DeleteModel(m_cylinderHandle);
		MV1DeleteModel(m_skyHandle);
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

void StageSetup::CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors)
{
	//�z�u�f�[�^���擾
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
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& charaData : characterData)
	{
		if (charaData.name == "Player")
		{
			m_player = std::make_shared<Player>(m_playerHandle, charaData.pos);
			m_player->GetModel()->SetScale(charaData.scale);
			m_player->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(m_player);
		}
		else if (charaData.name == "SmallDragon")
		{
			std::shared_ptr<EnemyBase> smallDragon = std::make_shared<SmallDragon>(MV1DuplicateModel(m_smallDragonHandle),
				charaData.pos);
			smallDragon->GetModel()->SetScale(charaData.scale);
			smallDragon->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(smallDragon);
		}
		else if (charaData.name == "BossDragon")
		{
			std::shared_ptr<EnemyBase> bossDragon = std::make_shared<BossDragon>(MV1DuplicateModel(m_bossDragonHandle),
				charaData.pos);
			bossDragon->GetModel()->SetScale(charaData.scale);
			bossDragon->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(bossDragon);
		}
		else if (charaData.name == "Bomber")
		{
			std::shared_ptr<EnemyBase> bomber = std::make_shared<Bomber>(MV1DuplicateModel(m_bomberHandle),
				charaData.pos);
			bomber->GetModel()->SetScale(charaData.scale);
			bomber->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(bomber);
		}
		else if (charaData.name == "PurpleDinosaur")
		{
			std::shared_ptr<EnemyBase> purpleDinosaur = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_purpleDinosaurHandle),
				charaData.pos);
			purpleDinosaur->GetModel()->SetScale(charaData.scale);
			purpleDinosaur->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(purpleDinosaur);
		}
	}
}

void StageSetup::CreateStage(std::vector<std::shared_ptr<Actor>>& actors)
{
	//����쐬
	actors.emplace_back(std::make_shared<Sky>(m_skyHandle));
	//�z�u�f�[�^���擾
	std::string path;
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		path = "Data/CSV/StageTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	//�`��p
	//�z�u�f�[�^���擾
	auto stageDrawData = TransformDataLoader::LoadDataCSV(path.c_str());
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> path =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_pathHandle), stageData.pos, stageData.scale, stageData.rot);
			actors.emplace_back(path);
		}
		/*else if (stageData.name == "Plane")
		{
			std::shared_ptr<StageObjectDraw> plane =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_cubeHandle), stageData.pos, stageData.scale, stageData.rot);
			actors.emplace_back(plane);
		}*/
	}
	////�����蔻��p
	////�z�u�f�[�^���擾
	//auto stageCollData = TransformDataLoader::LoadDataCSV("Data/CSV/StageCollisionTransformData.csv");
	////���O����R���W������z�u���Ă���
	//for (auto& stageData : stageCollData)
	//{
	//	if (stageData.name == "Cube")
	//	{
	//		std::shared_ptr<StageObjectCollision> Cube =
	//			std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_cubeHandle), stageData.pos, stageData.scale, stageData.rot);
	//		actors.emplace_back(Cube);
	//	}
	//	else if (stageData.name == "Cylinder")
	//	{
	//		std::shared_ptr<StageObjectCollision> cylinder =
	//			std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_cylinderHandle), stageData.pos, stageData.scale, stageData.rot);
	//		actors.emplace_back(cylinder);
	//	}
	//}
}

#include "ActorManager.h"
#include "Stage/BossArea.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"
#include "../UI/UIManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>
#include <cassert>
//�z�u�f�[�^
#include "../../General/CSVDataLoader.h"
//�A�N�^�[
#include "Actor.h"
//�v���C���[
#include "Character/Player/Player.h"
//�G
#include "Character/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "Character/Enemy/SmallDragon/SmallDragon.h"
#include "Character/Enemy/BossDragon/BossDragon.h"
#include "Character/Enemy/Bomber/Bomber.h"
#include "Character/Enemy/EnemyBase.h"
//�X�e�[�W
#include "Stage/InvisibleWall.h"
#include "Stage/StageObjectCollision.h"
#include "Stage/StageObjectDraw.h"
#include "Stage/Sky.h"
#include "Stage/BossArea.h"
//�A�C�e��
#include "Item/Heart.h"
#include "Item/UltGageUp.h"
#include "Item/Bomb.h"
#include "Item/AttackUp.h"
#include "Item/DefenseUp.h"
//�U��
#include "Attack/Slash.h"
#include "Attack/Strike.h"
#include "Attack/AreaOfEffectAttack.h"
#include "Attack/Bullet.h"
#include "Attack/Blast.h"
#include "Attack/Breath.h"

ActorManager::ActorManager(Stage::StageIndex index,std::weak_ptr<UIManager> uiManager):
	m_actorId(0),
	m_uiManager(uiManager)
{
	//�n���h�����[�h
	LoadHandle();
	//�X�e�[�W�������[�h
	LoadStage(index);
	//�U���̏����쐬
	CreateAttackData();
}

ActorManager::~ActorManager()
{
}

//�A�N�^�[��ǉ�
void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())return;
	//�A�N�^�[�̏�����
	actor->Init();
	//�A�N�^�[��ID��ݒ�
	actor->SetID(m_actorId);
	m_actorId++;
	//�A�N�^�[��ǉ�
	m_actors.emplace_back(actor);
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//�o�^����Ă��Ȃ��Ȃ炵�Ȃ�
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it == m_actors.end())return;
	actor->End();
	m_actors.remove(actor);
}


void ActorManager::Init()
{
	//�A�N�^�[������
	CheckNextAddActors();
}

void ActorManager::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<Score> score)
{
	//�V�K�A�N�^�[�̒ǉ�
	CheckNextAddActors();
	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(camera,shared_from_this());
	}
	//�{�X���|���ꂽ�Ƃ�
	if (m_boss.expired())
	{
		//�v���C���[�̗̑͂���X�R�A�����Z
		score.lock()->AddHPScore(m_player.lock()->GetHitPoints());
	}
	//���Ńt���O�`�F�b�N
	CheckDeleteActors(score);
}

void ActorManager::Draw() const
{
	//�A�N�^�[�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
}

void ActorManager::End()
{
	//�����������
	//�A�N�^�[�̏I������
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (auto& actor : m_actors)
	{
		deleteActer.emplace_back(actor);
	}
	for (auto& actor : deleteActer)
	{
		Exit(actor);
	}
	deleteActer.clear();
	m_nextAddActors.clear();
	m_player.reset();
	m_boss.reset();
	//�n���h��
	MV1DeleteModel(m_playerHandle);
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
	MV1DeleteModel(m_blockGrassHandle);
	MV1DeleteModel(m_heartHandle);
	MV1DeleteModel(m_bombHandle);
	MV1DeleteModel(m_ultGageUpHandle);
	MV1DeleteModel(m_attackUpHandle);
	MV1DeleteModel(m_defenseUpHandle);
}

//�V�K�A�N�^�[�̒ǉ��\����󂯎��(public)
void ActorManager::AddNextActor(std::shared_ptr<Actor> actor)
{
	//�ǉ��\��̃A�N�^�[��ǉ�
	m_nextAddActors.emplace_back(actor);
}

std::weak_ptr<CharacterBase> ActorManager::CreateCharacter(CharacterType ch, Vector3 pos)
{
	//�L�����N�^�[���쐬
	std::shared_ptr<CharacterBase> chara;
	//�K�v��UI���쐬���邽�߂�
	auto uiManager = m_uiManager.lock();
	switch (ch)
	{
	case CharacterType::Player:
		//�v���C���[�쐬
		chara = std::make_shared<Player>(m_playerHandle, pos);
		//�v���C���[�ɕK�v��UI�쐬
		uiManager->CreatePlayerUI(std::dynamic_pointer_cast<Player>(chara));
		break;
	case CharacterType::PurpleDinosaur:
		chara = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_purpleDinosaurHandle), pos);
		//�G�ɕK�v��UI�쐬
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::SmallDragon:
		chara = std::make_shared<SmallDragon>(MV1DuplicateModel(m_smallDragonHandle), pos);
		//�G�ɕK�v��UI�쐬
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::Bomber:
		chara = std::make_shared<Bomber>(MV1DuplicateModel(m_bomberHandle), pos);
		//�G�ɕK�v��UI�쐬
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::BossDragon:
		chara = std::make_shared<BossDragon>(MV1DuplicateModel(m_bossDragonHandle), pos);
		//�{�X�ɕK�v��UI�쐬
		uiManager->CreateBossUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	default:
		break;
	}
	//�L�����N�^�[������
	AddNextActor(chara);
	return chara;
}
std::weak_ptr<AttackBase> ActorManager::CreateAttack(AttackType at, std::weak_ptr<Actor> owner)
{
	//�U�����쐬
	std::shared_ptr<AttackBase> attack;
	switch (at)
	{
	case AttackType::Slash:
		attack = std::make_shared<Slash>(owner);
		break;
	case AttackType::Strike:
		attack = std::make_shared<Strike>(owner);
		break;
	case AttackType::AreaOfEffect:
		attack = std::make_shared<AreaOfEffectAttack>(owner);
		break;
	case AttackType::Bullet:
		attack = std::make_shared<Bullet>(owner);
		break;
	case AttackType::Blast:
		attack = std::make_shared<Blast>(owner);
		break;
	case AttackType::Breath:
		attack = std::make_shared<Breath>(owner);
		break;
	default:
		break;
	}
	//�U��������
	AddNextActor(attack);
	return attack;
}

std::weak_ptr<ItemBase> ActorManager::CreateItem(ItemType it, Vector3 pos)
{
	//�U�����쐬
	std::shared_ptr<ItemBase> item;
	switch (it)
	{
	case ItemType::Heart:
		item = std::make_shared<Heart>(MV1DuplicateModel(m_heartHandle), pos);
		break;
	case ItemType::Bomb:
		item = std::make_shared<Bomb>(MV1DuplicateModel(m_bombHandle), pos);
		break;
	case ItemType::UltGageUp:
		item = std::make_shared<UltGageUp>(MV1DuplicateModel(m_ultGageUpHandle), pos);
		break;
	case ItemType::AttackUp:
		item = std::make_shared<AttackUp>(MV1DuplicateModel(m_attackUpHandle), pos);
		break;
	case ItemType::DefenseUp:
		item = std::make_shared<DefenseUp>(MV1DuplicateModel(m_defenseUpHandle), pos);
		break;
	default:
		break;
	}
	//�A�C�e��������
	AddNextActor(item);
	return item;
}

void ActorManager::AllDeleteNormalEnemy()
{
	//�G
	for (auto& actor : m_actors)
	{
		//�G��T��
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			auto enemy = std::dynamic_pointer_cast<EnemyBase>(actor);
			//�{�X�ȊO�Ȃ�
			if (enemy->GetEnemyGrade() != EnemyGrade::Boss)
			{
				enemy->Delete();
			}
		}
	}
}

//�v���C���[�ɋ߂��G���擾
std::weak_ptr<Actor> ActorManager::GetNearestEnemy() const
{
	std::weak_ptr<Actor> nearestEnemy; //�߂��G�̃|�C���^
	float minDis = 1000000.0f; //�����l�͑傫�Ȓl
	for (auto& actor : m_actors)
	{
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			//�v���C���[�ɋ߂��G��T��
			float dis = (m_player.lock()->GetPos() - actor->GetPos()).Magnitude();
			if (dis < minDis)
			{
				minDis = dis;
				nearestEnemy = actor; //�߂��G���X�V
			}
		}
	}
	return nearestEnemy;
}

AttackData ActorManager::GetAttackData(std::string& ownerName, std::string& attackName)
{
	AttackData attackData;
	for (auto data : m_attackDatas)
	{
		//������ƍU���̖��O�������Ȃ�
		if (data.ownerName == ownerName && data.attackName == attackName)
		{
			attackData = data;
			break;
		}
	}
	return attackData;
}

//�A�N�^�[�̏��Ńt���O���`�F�b�N���č폜
void ActorManager::CheckDeleteActors(const std::weak_ptr<Score> score)
{
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		for (auto& actor : m_actors)
		{
			bool isDead = actor->IsDelete();//���S���������`�F�b�N
			if (isDead)
			{
				isOneMore = true;
				//���S�����A�N�^�[�̏I������
				actor->Dead(thisPointer, score);
				//�폜���
				deleteActer.emplace_back(actor);
			}
		}
		//�폜
		for (auto& actor : deleteActer)
		{
			thisPointer->Exit(actor);
		}
		deleteActer.clear();
		if (!isOneMore)break;
	}
}

//�ǉ��\��̃A�N�^�[������
void ActorManager::CheckNextAddActors()
{
	for (auto& actor : m_nextAddActors)
	{
		Entry(actor);
	}
	m_nextAddActors.clear();//�ǉ��\��̃A�N�^�[�͏���
}

//�U���f�[�^�̎擾
void ActorManager::CreateAttackData()
{
	m_attackDatas = CSVDataLoader::LoadAttackDataCSV("Data/CSV/CharacterAttackData.csv");
}

void ActorManager::LoadHandle()
{
	m_playerHandle = MV1LoadModel("Data/Model/Player/Player.mv1");
	assert(m_playerHandle >= 0);
	m_wallHandle = MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1");
	assert(m_wallHandle >= 0);
	m_purpleDinosaurHandle = MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1");
	assert(m_purpleDinosaurHandle >= 0);
	m_smallDragonHandle = MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1");
	assert(m_smallDragonHandle >= 0);
	m_bomberHandle = MV1LoadModel("Data/Model/Enemy/Bomber.mv1");
	assert(m_bomberHandle >= 0);
	m_bossDragonHandle = MV1LoadModel("Data/Model/Enemy/BossDragon.mv1");
	assert(m_bossDragonHandle >= 0);
	m_pathHandle = MV1LoadModel("Data/Model/Stage/1/Path.mv1");
	assert(m_pathHandle >= 0);
	m_blockGrassHandle = MV1LoadModel("Data/Model/Stage/1/Block_Grass.mv1");
	assert(m_blockGrassHandle >= 0);
	m_cubeHandle = MV1LoadModel("Data/Model/Collision/Cube.mv1");
	assert(m_cubeHandle >= 0);
	m_cylinderHandle = MV1LoadModel("Data/Model/Collision/Cylinder.mv1");
	assert(m_cylinderHandle >= 0);
	m_planeHandle = MV1LoadModel("Data/Model/Collision/Plane.mv1");
	assert(m_planeHandle >= 0);
	m_skyHandle = MV1LoadModel("Data/Model/Stage/Sky/Sky_Daylight02.pmx");
	assert(m_skyHandle >= 0);
	m_heartHandle = MV1LoadModel("Data/Model/Item/Heart.mv1");
	assert(m_heartHandle >= 0);
	m_bombHandle = MV1LoadModel("Data/Model/Item/Bomb.mv1");
	assert(m_bombHandle >= 0);
	m_ultGageUpHandle = MV1LoadModel("Data/Model/Item/UltGageUp.mv1");
	assert(m_ultGageUpHandle >= 0);
	m_attackUpHandle = MV1LoadModel("Data/Model/Item/AttackUp.mv1");
	assert(m_attackUpHandle >= 0);
	m_defenseUpHandle = MV1LoadModel("Data/Model/Item/DefenseUp.mv1");
	assert(m_defenseUpHandle >= 0);
}
void ActorManager::LoadStage(Stage::StageIndex index)
{
	//�z�u�f�[�^���擾
	std::string charaPath;
	std::string drawPath;
	std::string collPath;
	std::string bossAreaPath;
	switch (index)
	{
	case Stage::StageIndex::Stage1:
		charaPath = "Data/CSV/CharacterTransformData.csv";
		drawPath = "Data/CSV/StageTransformData.csv";
		collPath = "Data/CSV/StageCollisionTransformData.csv";
		bossAreaPath = "Data/CSV/BossTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	auto characterData = CSVDataLoader::LoadTransformDataCSV(charaPath.c_str());
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& charaData : characterData)
	{
		if (charaData.name == "Player")
		{
			auto player = CreateCharacter(CharacterType::Player, charaData.pos).lock();
			player->GetModel()->SetScale(charaData.scale);
			player->GetModel()->SetRot(charaData.rot);
			m_player = std::dynamic_pointer_cast<Player>(player);
		}
		else if (charaData.name == "SmallDragon")
		{
			auto smallDragon = CreateCharacter(CharacterType::SmallDragon, charaData.pos).lock();
			smallDragon->GetModel()->SetScale(charaData.scale);
			smallDragon->GetModel()->SetRot(charaData.rot);
		}
		else if (charaData.name == "BossDragon")
		{
			auto bossDragon = CreateCharacter(CharacterType::BossDragon, charaData.pos).lock();
			bossDragon->GetModel()->SetScale(charaData.scale);
			bossDragon->GetModel()->SetRot(charaData.rot);
			m_boss = std::dynamic_pointer_cast<BossDragon>(bossDragon);

		}
		else if (charaData.name == "Bomber")
		{
			auto bomber = CreateCharacter(CharacterType::Bomber, charaData.pos).lock();
			bomber->GetModel()->SetScale(charaData.scale);
			bomber->GetModel()->SetRot(charaData.rot);
		}
		else if (charaData.name == "PurpleDinosaur")
		{
			auto purpleDinosaur = CreateCharacter(CharacterType::PurpleDinosaur, charaData.pos).lock();
			purpleDinosaur->GetModel()->SetScale(charaData.scale);
			purpleDinosaur->GetModel()->SetRot(charaData.rot);
		}
	}
	//����쐬
	m_actors.emplace_back(std::make_shared<Sky>(m_skyHandle));
	//�`��p
	//�z�u�f�[�^���擾
	auto stageDrawData = CSVDataLoader::LoadTransformDataCSV(drawPath.c_str());
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> path =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_pathHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(path);
		}
		else if (stageData.name == "Block_Grass")
		{
			//�傫����1/100���Ȃ��Ƒ傫������̂�
			stageData.scale = VScale(stageData.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_blockGrassHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(blockGrass);
		}
	}
	//�����蔻��p
	//�z�u�f�[�^���擾
	auto stageCollData = CSVDataLoader::LoadTransformDataCSV(collPath.c_str());
	//���O����R���W������z�u���Ă���
	for (auto& stageData : stageCollData)
	{
		if (stageData.name == "Plane")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(plane);
		}
		else if (stageData.name == "BossAreaWall")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_planeHandle), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(plane);
		}
	}
	//�{�X�������쐬
	auto bossAreaData = CSVDataLoader::LoadTransformDataCSV(bossAreaPath.c_str());
	VECTOR startPos = {};
	VECTOR endPos = {};
	//���O����R���W������z�u���Ă���
	for (auto& stageData : bossAreaData)
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
	auto bossArea = std::make_shared<BossArea>(startPos, endPos);
	m_bossArea = bossArea;
	m_nextAddActors.emplace_back(bossArea);
}
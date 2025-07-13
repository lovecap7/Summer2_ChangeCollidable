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
#include "Stage/EventArea.h"
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

namespace
{
	constexpr int kAreaPartsNum = 2;
}

ActorManager::ActorManager(Stage::StageIndex index,std::weak_ptr<UIManager> uiManager):
	m_actorId(0),
	m_uiManager(uiManager)
{
	m_csvLoader = std::unique_ptr<CSVDataLoader>();
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
	//�n���h�������ׂč폜
	for (auto& [key, value] : m_handles) {
		if (value >= 0)
		{
			auto result = MV1DeleteModel(value);
			assert(result == 0);
		}
	}
	m_handles.clear();
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
		chara = std::make_shared<Player>(MV1DuplicateModel(m_handles["Player"]), pos);
		//�v���C���[�ɕK�v��UI�쐬
		uiManager->CreatePlayerUI(std::dynamic_pointer_cast<Player>(chara));
		break;
	case CharacterType::PurpleDinosaur:
		chara = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_handles["PurpleDinosaur"]), pos);
		//�G�ɕK�v��UI�쐬
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::SmallDragon:
		chara = std::make_shared<SmallDragon>(MV1DuplicateModel(m_handles["SmallDragon"]), pos);
		//�G�ɕK�v��UI�쐬
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::Bomber:
		chara = std::make_shared<Bomber>(MV1DuplicateModel(m_handles["Bomber"]), pos);
		//�G�ɕK�v��UI�쐬
		uiManager->CreateEnemyUI(std::dynamic_pointer_cast<EnemyBase>(chara));
		break;
	case CharacterType::BossDragon:
		chara = std::make_shared<BossDragon>(MV1DuplicateModel(m_handles["BossDragon"]), pos);
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
		item = std::make_shared<Heart>(MV1DuplicateModel(m_handles["Heart"]), pos);
		break;
	case ItemType::Bomb:
		item = std::make_shared<Bomb>(MV1DuplicateModel(m_handles["Bomb"]), pos);
		break;
	case ItemType::UltGageUp:
		item = std::make_shared<UltGageUp>(MV1DuplicateModel(m_handles["UltGageUp"]), pos);
		break;
	case ItemType::AttackUp:
		item = std::make_shared<AttackUp>(MV1DuplicateModel(m_handles["AttackUp"]), pos);
		break;
	case ItemType::DefenseUp:
		item = std::make_shared<DefenseUp>(MV1DuplicateModel(m_handles["DefenseUp"]), pos);
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
	m_attackDatas = m_csvLoader->LoadAttackDataCSV("Data/CSV/CharacterAttackData.csv");
}

void ActorManager::LoadHandle()
{
	m_handles["Player"]			= { MV1LoadModel("Data/Model/Player/Player.mv1") };
	m_handles["InvisibleWall"]	= { MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1") };
	m_handles["PurpleDinosaur"] = { MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1") };
	m_handles["SmallDragon"]	= { MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1") };
	m_handles["Bomber"]			= { MV1LoadModel("Data/Model/Enemy/Bomber.mv1") };
	m_handles["BossDragon"]		= { MV1LoadModel("Data/Model/Enemy/BossDragon.mv1") };
	m_handles["Path"]			= { MV1LoadModel("Data/Model/Stage/1/Path.mv1") };
	m_handles["Block_Grass"]	= { MV1LoadModel("Data/Model/Stage/1/Block_Grass.mv1") };
	m_handles["Cube"]			= { MV1LoadModel("Data/Model/Collision/Cube.mv1")};
	m_handles["Cylinder"]		= { MV1LoadModel("Data/Model/Collision/Cylinder.mv1") };
	m_handles["Plane"]			= { MV1LoadModel("Data/Model/Collision/Plane.mv1") };
	m_handles["Sky"]			= { MV1LoadModel("Data/Model/Stage/Sky/Sky_Daylight02.pmx") };
	m_handles["Heart"]			= { MV1LoadModel("Data/Model/Item/Heart.mv1") };
	m_handles["Bomb"]			= { MV1LoadModel("Data/Model/Item/Bomb.mv1") };
	m_handles["UltGageUp"]		= { MV1LoadModel("Data/Model/Item/UltGageUp.mv1") };
	m_handles["AttackUp"]		= { MV1LoadModel("Data/Model/Item/AttackUp.mv1") };
	m_handles["DefenseUp"]		= { MV1LoadModel("Data/Model/Item/DefenseUp.mv1") };
	//���[�h�ɐ����������`�F�b�N
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}
void ActorManager::LoadStage(Stage::StageIndex index)
{
	//�z�u�f�[�^���擾
	std::string charaPath;
	std::string drawPath;
	std::string collPath;
	std::string eventAreaPath;
	switch (index)
	{
	case Stage::StageIndex::Stage1:
		charaPath = "Data/CSV/CharacterTransformData.csv";
		drawPath = "Data/CSV/StageTransformData.csv";
		collPath = "Data/CSV/StageCollisionTransformData.csv";
		eventAreaPath = "Data/CSV/EventTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		break;
	case Stage::StageIndex::Stage3:
		break;
	default:
		break;
	}
	auto characterData = m_csvLoader->LoadTransformDataCSV(charaPath.c_str());
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
	m_actors.emplace_back(std::make_shared<Sky>(MV1DuplicateModel(m_handles["Sky"])));
	//�`��p
	//�z�u�f�[�^���擾
	auto stageDrawData = m_csvLoader->LoadTransformDataCSV(drawPath.c_str());
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> path =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Path"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(path);
		}
		else if (stageData.name == "Block_Grass")
		{
			//�傫����1/100���Ȃ��Ƒ傫������̂�
			stageData.scale = VScale(stageData.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Block_Grass"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(blockGrass);
		}
	}
	//�����蔻��p
	//�z�u�f�[�^���擾
	auto stageCollData = m_csvLoader->LoadTransformDataCSV(collPath.c_str());
	//���O����R���W������z�u���Ă���
	for (auto& stageData : stageCollData)
	{
		if (stageData.name == "Plane" || stageData.name == "BossAreaWall")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(plane);
		}
	}
	//�C�x���g�������쐬
	auto eventAreaData = m_csvLoader->LoadTransformDataCSV(eventAreaPath.c_str());
	std::list<std::shared_ptr<StageObjectCollision>> eventAreaParts;
	std::list<std::shared_ptr<StageObjectCollision>> bossAreaParts;
	//���O����R���W������z�u���Ă���
	for (auto& stageData : eventAreaData)
	{
		if (stageData.name == "EventAreaS" || stageData.name == "EventAreaE")
		{
			auto coll = std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			eventAreaParts.emplace_back(coll);
			m_nextAddActors.emplace_back(coll);
		}
		else if (stageData.name == "BossAreaS" || stageData.name == "BossAreaE")
		{
			auto coll = std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			bossAreaParts.emplace_back(coll);
			m_nextAddActors.emplace_back(coll);
		}
		//�G���A���\������2�v�f����������
		if (eventAreaParts.size() >= kAreaPartsNum)
		{
			auto eventArea = std::make_shared<EventArea>(eventAreaParts.front(), eventAreaParts.back());
			m_nextAddActors.emplace_back(eventArea);
			eventAreaParts.clear();
		}
		if (bossAreaParts.size() >= kAreaPartsNum)
		{
			auto bossArea = std::make_shared<BossArea>(bossAreaParts.front(), bossAreaParts.back());
			m_bossArea = bossArea;
			m_nextAddActors.emplace_back(bossArea);
			bossAreaParts.clear();
		}
	}
}
#include "ActorManager.h"
#include "../StageSetup.h"
#include "Actor.h"
#include "Character/Player/Player.h"
#include "Character/Enemy/EnemyBase.h"
#include "Stage/BossArea.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"
#include "Attack/Slash.h"
#include "Attack/Strike.h"
#include "Attack/AreaOfEffectAttack.h"
#include "Attack/Bullet.h"
#include "Attack/Blast.h"
#include "Attack/Breath.h"
#include "Item/Heart.h"
#include "Item/UltGageUp.h"
#include "Item/Bomb.h"
#include "Item/AttackUp.h"
#include "Item/DefenseUp.h"
#include <DxLib.h>

ActorManager::ActorManager():
	m_actorId(0),
	m_heartHandle(-1),
	m_bombHandle(-1),
	m_ultGageUpHandle(-1),
	m_attackUpHandle(-1),
	m_defenseUpHandle(-1)
{
}

ActorManager::~ActorManager()
{
}

void ActorManager::Init(std::unique_ptr<StageSetup>& stageSetup)
{
	std::list<std::shared_ptr<Actor>> actors;
	//�A�N�^�[���󂯎��
	stageSetup->MoveActorsPointer(actors);
	//�A�N�^�[�̏���������
	for (auto& actor : actors)
	{
		//�ǉ���ID�̊���U��
		AddActor(actor);
	}
	actors.clear();//�󂯎�����A�N�^�[�͏���
	//�v���C���[�̎Q��
	for (auto& actor : m_actors)
	{
		//�v���C���[��T��
		if (actor->GetGameTag() == GameTag::Player)
		{
			m_player = std::dynamic_pointer_cast<Player>(actor);
			break;
		}
	}
	//�{�X�̎Q��
	for (auto& actor : m_actors)
	{
		//�G��T��
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			auto enemy = std::dynamic_pointer_cast<EnemyBase>(actor);
			//�{�X�Ȃ�
			if (enemy->GetEnemyGrade() == EnemyGrade::Boss)
			{
				m_boss = enemy;
				break;
			}
		}
	}
	//�{�X�����̎Q��
	for (auto& actor : m_actors)
	{
		//�{�X������T��
		if (actor->GetGameTag() == GameTag::Area)
		{
			m_bossArea = std::dynamic_pointer_cast<BossArea>(actor);
			break;
		}
	}
	//�n���h��
	m_heartHandle = MV1LoadModel("Data/Model/Item/Heart.mv1");
	m_bombHandle = MV1LoadModel("Data/Model/Item/Bomb.mv1");
	m_ultGageUpHandle = MV1LoadModel("Data/Model/Item/UltGageUp.mv1");
	m_attackUpHandle = MV1LoadModel("Data/Model/Item/AttackUp.mv1");
	m_defenseUpHandle = MV1LoadModel("Data/Model/Item/DefenseUp.mv1");
}

void ActorManager::Update(const std::weak_ptr<Camera> camera)
{
	//���Ńt���O�`�F�b�N
	CheckDeleteActors();
	//�V�K�A�N�^�[�̒ǉ�
	CheckNextAddActors();
	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(camera,shared_from_this());
	}
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
	//�A�N�^�[�̏I������
	for (auto& actor : m_actors)
	{
		actor->End();
	}
	//�����������
	m_actors.clear();
	m_player.reset();
	//�n���h��
	MV1DeleteModel(m_heartHandle);
	MV1DeleteModel(m_bombHandle);
	MV1DeleteModel(m_ultGageUpHandle);
	MV1DeleteModel(m_attackUpHandle);
	MV1DeleteModel(m_defenseUpHandle);
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

//�V�K�A�N�^�[�̒ǉ��\����󂯎��(public)
void ActorManager::AddNextActor(std::shared_ptr<Actor> actor)
{
	//�ǉ��\��̃A�N�^�[��ǉ�
	m_nextAddActors.emplace_back(actor);
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

//�A�N�^�[��ǉ�
void ActorManager::AddActor(std::shared_ptr<Actor> actor)
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

//�A�N�^�[�̏��Ńt���O���`�F�b�N���č폜
void ActorManager::CheckDeleteActors()
{
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [thisPointer, &isOneMore](std::shared_ptr<Actor> actor) {
			bool isDead = actor->IsDelete();//���S���������`�F�b�N
			if (isDead)
			{
				isOneMore = true;
				//���S�����A�N�^�[�̏I������
				actor->Dead(thisPointer);
				actor->End();
			}
			return isDead;
			});
		m_actors.erase(remIt, m_actors.end());//�폜
		if (!isOneMore)break;
	}
}

//�ǉ��\��̃A�N�^�[������
void ActorManager::CheckNextAddActors()
{
	for (auto& actor : m_nextAddActors)
	{
		AddActor(actor);
	}
	m_nextAddActors.clear();//�ǉ��\��̃A�N�^�[�͏���
}

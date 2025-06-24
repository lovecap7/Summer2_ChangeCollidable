#include "ActorManager.h"
#include "Actor.h"
#include "Player/Player.h"
#include "../StageSetup.h"
#include "../../General/Input.h"
#include "../Camera/Camera.h"
#include "../Attack/AttackManager.h"
#include "Enemy/EnemyManager.h"
#include "Player/PlayerManager.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collision/CollisionManager.h"
#include "../../General/game.h"
#include "../UI/UIManager.h"
#include "Item/ItemGenerator.h"

ActorManager::ActorManager(std::shared_ptr<Player> player):
	m_actors{},
	m_addActors{},
	m_player(player),
	m_id(0)
{
	//�R���W�����}�l�[�W���[
	m_collManager = std::make_shared<CollisionManager>();
	//�G�l�~�[�}�l�[�W���[
	m_enemyManager = std::make_shared<EnemyManager>(m_player);
	//�v���C���[�}�l�[�W���[
	m_playerManager = std::make_shared<PlayerManager>(m_player);
	//�U���̏���
	m_attackManager = std::make_shared<AttackManager>();
	//�A�C�e���W�F�l���[�^�[
	m_itemGenerator = std::make_shared<ItemGenerator>();
}

ActorManager::~ActorManager()
{
}

void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		return;
	}
	//ID���Z�b�g
	SetUpId(actor);
	//�A�N�^�[���Z�b�g
	m_actors.emplace_back(actor);
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//���łɓo�^����Ă���Ȃ炻�̃A�N�^�[������
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		//�폜
		m_actors.erase(it);
	}
}

void ActorManager::Init(std::shared_ptr<StageSetup> setup)
{
	//�A�N�^�[���󂯎��
	setup->MoveActorsPointer(m_addActors);
	//�A�N�^�[�̓o�^
	for (auto& actor : m_addActors)
	{
		actor->Entry(shared_from_this());
	}
	//�A�N�^�[�̏���������
	for (auto& actor : m_actors)
	{
		actor->Init();
	}
	m_addActors.clear(); //�ǉ��\��̃A�N�^�[���N���A
}

void ActorManager::End()
{
	//�A�N�^�[�̏I������
	for (auto& actor : m_actors)
	{
		actor->Exit(shared_from_this());
	}
	//�A�C�e���̏I������
	m_itemGenerator->End();
}

void ActorManager::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<UIManager> uiManager)
{
	//�v���C���[�̍��G
	m_enemyManager->Update();
	//�G�̍��G
	m_playerManager->Update(m_actors);
	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(input, camera, shared_from_this());
		actor->Gravity(Gravity::kGravity);
	}
	//�U���̏���
	m_attackManager->Update(m_actors);

	//���Ńt���O�`�F�b�N
	CheckDeleteActor(m_itemGenerator);

	//�A�N�^�[�̏Փˏ���
	m_collManager->Update(m_actors);

	//���Ńt���O�`�F�b�N
	CheckDeleteActor(m_itemGenerator);

	//�X�V�m��
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}

	//�Q�[�����ɐV�����A�N�^�[���ǉ����ꂽ�ꍇ
	//��������
	AddNewActors();
}

void ActorManager::Draw() const
{
	//�A�N�^�[�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
	//�U���̕`��
	m_attackManager->Draw();
}

void ActorManager::SetNewActor(std::shared_ptr<Actor> actor)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())
	{
		return;
	}
	//�ǉ��\��̃A�N�^�[�ɒǉ�
	m_addActors.emplace_back(actor);
}

void ActorManager::SetUpId(std::shared_ptr<Actor> actor)
{
	//ID��o�^
	actor->SetID(m_id);
	++m_id;
}

void ActorManager::CheckDeleteActor(std::shared_ptr<ItemGenerator> itemGenerator)
{
	//���Ńt���O�`�F�b�N
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [itemGenerator](std::shared_ptr<Actor> actor) {
		bool isDead = actor->IsDelete();
		if (isDead)
		{
			//�G�����ł����Ƃ�
			if (actor->GetCollidable()->GetGameTag() == GameTag::Enemy)
			{
				//�A�C�e���������_������
				itemGenerator->RandGenerateItem(actor->GetCollidable()->GetRb()->GetPos());
			}
		}
		return isDead;
		});
	m_actors.erase(remIt, m_actors.end());//�폜
}
void ActorManager::AddNewActors()
{
	//�ǉ��\��̃A�C�e��������
	m_itemGenerator->MoveItems(shared_from_this());
	//�ǉ��\��̃A�N�^�[������
	if (!m_addActors.empty())
	{
		for (auto& actor : m_addActors)
		{
			//�A�N�^�[�̓o�^
			Entry(actor);
		}
		m_addActors.clear();//�ǉ��\��̃A�N�^�[���N���A
	}
}
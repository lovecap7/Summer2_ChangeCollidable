#include "ActorManager.h"
#include "../StageSetup.h"
#include "Actor.h"
#include "Character/Player/Player.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"

ActorManager::ActorManager():
	m_actorId(0)
{
	//�X�e�[�W���쐬
	m_stageSetup = std::make_shared<StageSetup>();
}

ActorManager::~ActorManager()
{
}

void ActorManager::Init()
{
	//�v���C���[���󂯎��
	m_stageSetup->MovePlayerPointer(m_player);
	std::list<std::shared_ptr<Actor>> actors;
	//�A�N�^�[���󂯎��
	m_stageSetup->MoveActorsPointer(actors);
	//�A�N�^�[�̏���������
	for (auto& actor : actors)
	{
		AddActor(actor);
	}
	actors.clear();//�󂯎�����A�N�^�[�͏���
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
	//�X�e�[�W�Z�b�g�A�b�v�̏I��
	m_stageSetup->End();
	m_stageSetup.reset();
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
			float dis = (m_player->GetPos() - actor->GetPos()).Magnitude();
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
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> actor) {
		bool isDead = actor->IsDelete();//���S���������`�F�b�N
		if (isDead)
		{
			//���S�����A�N�^�[�̏I������
			actor->End();
		}
		return isDead;
		});
	m_actors.erase(remIt, m_actors.end());//�폜
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

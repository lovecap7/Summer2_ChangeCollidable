#include "BossArea.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/Camera.h"
BossArea::BossArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	EventArea(start,end),
	m_isEntryBossArea(false),
	m_update(&BossArea::EntryCheckUpdate)
{
}

BossArea::~BossArea()
{
}

void BossArea::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera, actorManager);
}

void BossArea::End()
{
}

void BossArea::EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	//���W����͈͂ɓ����������`�F�b�N
	auto playerPos = player->GetPos();
	auto startPos = m_start.lock()->GetPos();
	auto endPos = m_end.lock()->GetPos();
	//�͈͓��Ȃ�
	if (playerPos.x > startPos.x && playerPos.x < endPos.x)
	{
		//�C�x���g�J�n�����J�����ɐݒ�
		camera.lock()->SetEventArea(std::dynamic_pointer_cast<EventArea>(shared_from_this()));
		//�C�x���g�t���O
		m_isEvent = true;
		//�{�X�G���A�ɓ������t���O
		m_isEntryBossArea = true;
		//�{�X�ȊO�̎G���G���폜
		actorManager.lock()->AllDeleteNormalEnemy();
		m_update = &BossArea::EventUpdate;
		return;
	}
}

void BossArea::EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�{�X���|�ꂽ��
	if (actorManager.lock()->GetBoss().expired())
	{
		//�͈͓��̓G�����ׂď�������
		//�C�x���g�I��
		m_isEvent = false;
		//���̃G���A������
		m_isDelete = true;
	}
}

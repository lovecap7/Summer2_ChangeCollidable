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
		camera.lock()->EventStart(startPos.x, endPos.x);
		//�{�X�G���A�ɓ������t���O
		m_isEntryBossArea = true;
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
		//�J�����ɃC�x���g���I���������Ƃ�ݒ�
		camera.lock()->EventEnd();
	}
}

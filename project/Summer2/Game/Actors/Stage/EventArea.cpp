#include "EventArea.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/Camera.h"

EventArea::EventArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	Actor(Shape::None),
	m_start(start),
	m_end(end)
{
}

EventArea::~EventArea()
{
}

void EventArea::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera,actorManager);
}

void EventArea::End()
{
}

void EventArea::EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		//�͈͓���Collidable�̎Q�Ƃ��擾
		auto collList = Physics::GetInstance().GetAreaXCollidable(startPos.x, endPos.x);
		for (auto& coll : collList)
		{
			//�͈͓��̓G�̎Q�Ƃ��擾
			if (coll.expired())continue;
			if (coll.lock()->GetGameTag() == GameTag::Enemy)
			{
				//�G
				m_areaEnemies.emplace_back(std::dynamic_pointer_cast<EnemyBase>(coll.lock()));
			}
		}
		//�C�x���g�J�n�����J�����ɐݒ�
		camera.lock()->EventStart(startPos.x, endPos.x);
		m_update = &EventArea::EventUpdate;
		return;
	}
}

void EventArea::EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�͈͓��̓G�����ׂĎ��S�����ꍇ�C�x���g���I��
	for (auto coll : m_areaEnemies)
	{
		//�͈͓��̓G���܂������Ă���Ȃ�
		if (!coll.expired())return;
	}
	//�͈͓��̓G�����ׂď�������
	//�J�����ɃC�x���g���I���������Ƃ�ݒ�
	camera.lock()->EventEnd();
	//�폜
	m_isDelete = true;
	//�ǂ��폜
	m_start.lock()->Delete();
	m_end.lock()->Delete();
}

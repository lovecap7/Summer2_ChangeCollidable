#include "BossArea.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"

BossArea::BossArea(VECTOR startPos, VECTOR endPos):
	Actor(Shape::None),
	m_startPos(startPos),
	m_endPos(endPos)
{
	//�I�_���n�_�̂ق������_��藣��Ă���Ȃ�
	if (abs(m_startPos.x) > abs(m_endPos.x))
	{
		//����ւ�
		auto temp = m_startPos;
		m_startPos = m_endPos;
		m_endPos = temp;
	}
}

BossArea::~BossArea()
{
	
}

void BossArea::Init()
{
	//�^�O���G���A��
	m_tag = GameTag::Area;
}

void BossArea::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�v���C���[�̃|�C���^
	auto player = actorManager.lock()->GetPlayer();
	if (player.expired())return;
	//�͈͓��Ƀv���C���[�����邩
	auto playerPosX = player.lock()->GetNextPos().x;
	if (playerPosX >= m_startPos.x && playerPosX <= m_endPos.x)
	{
		m_isDelete = true;
	}
}

void BossArea::Draw() const
{
	DrawLine3D(m_startPos.ToDxLibVector(), m_endPos.ToDxLibVector(), 0xff0000);
}

void BossArea::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�{�X�ȊO���폜
	actorManager.lock()->AllDeleteNormalEnemy();
}

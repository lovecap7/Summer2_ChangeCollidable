#include "BossArea.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"

BossArea::BossArea(VECTOR startPos, VECTOR endPos):
	Actor(Shape::None),
	m_startPos(startPos),
	m_endPos(endPos)
{
	//終点より始点のほうが原点より離れているなら
	if (abs(m_startPos.x) > abs(m_endPos.x))
	{
		//入れ替え
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
	//タグをエリアに
	m_tag = GameTag::Area;
}

void BossArea::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーのポインタ
	auto player = actorManager.lock()->GetPlayer();
	if (player.expired())return;
	//範囲内にプレイヤーがいるか
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
	//ボス以外を削除
	actorManager.lock()->AllDeleteNormalEnemy();
}

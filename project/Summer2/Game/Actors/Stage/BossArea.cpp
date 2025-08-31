#include "BossArea.h"
#include "StageObjectCollision.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../UI/UIManager.h"
#include "../../../General/Collision/Physics.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../Camera/GameCamera/GameCamera.h"

namespace
{
	//アイテムの生成間隔
	constexpr int kItemDropInterval = 20 * 60;
	//アイテムの生成位置
	constexpr float kItemDropHeight = 2000.0f;
}

BossArea::BossArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end):
	EventAreaBase(start,end,AreaTag::Boss),
	m_update(&BossArea::EntryCheckUpdate),
	m_itemDropFrame(0)
{
}

BossArea::~BossArea()
{
}

void BossArea::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera, actorManager);
}

void BossArea::EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (actorManager.expired())return;
	auto boss = actorManager.lock()->GetBoss();
	if (boss.expired())return;
	//ボスを非アクティブ状態に
	boss.lock()->SetActive(false);
	//プレイヤーが入ったかチェック
	EventAreaBase::Update(camera, actorManager);
	//入ったなら
	if (m_isEvent)
	{
		//ボスをアクティブ状態に
		boss.lock()->SetActive(true);
		//ボスのUIの準備
		UIManager::GetInstance().CreateBossUI(boss.lock());
		InitEvent(actorManager, camera);
		return;
	}
}

void BossArea::EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが倒れたら
	if (actorManager.lock()->GetBoss().expired())
	{
		//範囲内の敵がすべて消えたら
		//イベント終了
		m_isEvent = false;
		//このエリアも消す
		m_isDelete = true;
		return;
	}
	//プレイヤーがいない場合は何もしない
	if (actorManager.lock()->GetPlayer().expired())return;
	//アイテムを落とすフレームをカウント
	++m_itemDropFrame;
	if (m_itemDropFrame >= kItemDropInterval)
	{
		m_itemDropFrame = 0;
		//プレイヤーとボスの間にアイテムを落とす
		auto player = actorManager.lock()->GetPlayer().lock();
		auto boss = actorManager.lock()->GetBoss().lock();
		//プレイヤーからボスへのベクトル
		Vector3 toBoss = boss->GetPos() - player->GetPos();
		float distance = toBoss.Magnitude();
		if(distance > 0.0f)
		{
			//正規化
			toBoss = toBoss.Normalize();
			//プレイヤーとボスの間の位置
			Vector3 dropPos = player->GetPos() + (toBoss * MyMath::GetRandF(0.0f,distance));
			//高いところから落とす
			dropPos.y += kItemDropHeight;
			//アイテムを落とす
			actorManager.lock()->CreateRandItem(dropPos);
		}
	}
}


void BossArea::InitEvent(const std::weak_ptr<ActorManager>& actorManager, const std::weak_ptr<GameCamera>& camera)
{
	//BGM変更
	auto& soundManager = SoundManager::GetInstance();
	switch (actorManager.lock()->GetStageIndex())
	{
	case Stage::StageIndex::Stage1:
		soundManager.PlayBGM("Stage1_BossBGM");
		break;
	case Stage::StageIndex::Stage2:
		soundManager.PlayBGM("Stage2_BossBGM");
		break;
	case Stage::StageIndex::Stage3:
		soundManager.PlayBGM("Stage3_BossBGM");
		break;
	}
	//壁は閉ざす
	std::dynamic_pointer_cast<StageObjectCollision>(m_start.lock())->SetIsThrough(false);
	std::dynamic_pointer_cast<StageObjectCollision>(m_end.lock())->SetIsThrough(false);
	//イベント開始情報をカメラに設定
	camera.lock()->SetEventArea(std::dynamic_pointer_cast<BossArea>(shared_from_this()));
	//ボス以外の雑魚敵を削除
	actorManager.lock()->AllDeleteNormalEnemy();
	m_update = &BossArea::EventUpdate;
}
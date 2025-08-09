#include "BossKingStateBase.h"
#include "BossKingStateMagicWave.h"
#include "BossKingStateBlastAttack.h"
#include "BossKingStateBeam.h"
#include "BossKing.h"

BossKingStateBase::BossKingStateBase(std::weak_ptr<Actor> owner, bool isTransformSecond):
	CharacterStateBase(owner),
	m_isTransformSecond(isTransformSecond)
{
}

BossKingStateBase::~BossKingStateBase()
{
}


void BossKingStateBase::ThinkAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//‹——£‚ğŒ©‚ÄUŒ‚‚ğŒˆ‚ß‚é
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//‹ß‹——£‚É‚¢‚é‚È‚ç
	if (targetData.targetDis <= kMeleeAttackDistance)
	{
		//UŒ‚‚Ì”
		int attackNum = 2;
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossKingStateMagicWave>(m_owner, actorManager));
			break;
		case 2:
			ChangeState(std::make_shared<BossKingStateBlastAttack>(m_owner, actorManager));
			break;
		}
		return;
	}
	//‰“‹——£‚È‚ç
	else
	{
		//UŒ‚‚Ì”
		int attackNum = 3;
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossKingStateMagicWave>(m_owner, actorManager));
			break;
		case 2:
			ChangeState(std::make_shared<BossKingStateBeam>(m_owner, actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossKingStateBlastAttack>(m_owner, actorManager));
			break;
		}
		return;
	}
}
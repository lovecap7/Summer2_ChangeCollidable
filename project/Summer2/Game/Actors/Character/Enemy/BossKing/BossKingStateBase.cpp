#include "BossKingStateBase.h"
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
	
	}
	//‰“‹——£‚È‚ç
	else
	{
		
	}
}
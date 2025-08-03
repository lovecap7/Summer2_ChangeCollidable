#include "BossMuscleStateBase.h"
#include "BossMuscleStateRightPunch.h"
#include "BossMuscleStateBeam.h"
#include "BossMuscleStateJumpAttack.h"
#include "BossMuscle.h"

BossMuscleStateBase::BossMuscleStateBase(std::weak_ptr<Actor> owner, bool isAngry) :
	CharacterStateBase(owner),
	m_isAngry(isAngry)
{
}

BossMuscleStateBase::~BossMuscleStateBase()
{
}


void BossMuscleStateBase::ThinkAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//‹——£‚ğŒ©‚ÄUŒ‚‚ğŒˆ‚ß‚é
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//‹ß‹——£‚É‚¢‚é‚È‚ç
	if (targetData.targetDis <= kMeleeAttackDistance)
	{
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(0,1);
		switch (rand)
		{
		case 0:
			ChangeState(std::make_shared<BossMuscleStateRightPunch>(m_owner, m_isAngry, actorManager));
			break;
		case 1:
			ChangeState(std::make_shared<BossMuscleStateJumpAttack>(m_owner, m_isAngry, actorManager));
			break;
		}
		return;
	}
	//‰“‹——£‚È‚ç
	else
	{
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(0, 1);
		switch (rand)
		{
		case 0:
			ChangeState(std::make_shared<BossMuscleStateBeam>(m_owner, m_isAngry, actorManager));
			break;
		case 1:
			ChangeState(std::make_shared<BossMuscleStateJumpAttack>(m_owner, m_isAngry, actorManager));
			break;
		}
		return;
	}
}

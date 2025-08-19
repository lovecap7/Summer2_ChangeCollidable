#include "GroupManager.h"
#include "Actor.h"
#include "../../General/Model.h"

GroupManager::GroupManager()
{
}

GroupManager::~GroupManager()
{
}

void GroupManager::Entry(std::shared_ptr<Actor> actor)
{
	std::string groupName = actor->GetGroupTag();
	//ƒAƒNƒ^[‚ğ’Ç‰Á
	m_groupActors[groupName].emplace_back(actor);
	//‚Ü‚¾“o˜^‚³‚ê‚Ä‚¢‚È‚¢ƒOƒ‹[ƒv–¼‚È‚ç
	auto it = std::find(m_groupNames.begin(), m_groupNames.end(), groupName);
	if (it == m_groupNames.end())
	{
		//ƒOƒ‹[ƒv–¼“o˜^
		m_groupNames.emplace_back(groupName);
		//UŒ‚Œ ‚ğ‚½‚¹‚é
		TakeTurnCanAttack(actor, groupName);
#if _DEBUG
		m_groupColor[groupName] = GetColor(MyMath::GetRand(0, 255), MyMath::GetRand(0, 255), MyMath::GetRand(0, 255));
#endif
	}
	else
	{
		//UŒ‚Œ ‚ğ‚È‚­‚·
		actor->SetCanAttack(false);
	}
}

void GroupManager::Exit(std::shared_ptr<Actor> actor)
{
	std::string groupName = actor->GetGroupTag();
	//“o˜^‚³‚ê‚Ä‚¢‚é‚È‚ç
	auto it = std::find(m_groupActors[groupName].begin(), m_groupActors[groupName].end(), actor);
	if (it == m_groupActors[groupName].end())return;
	//‚à‚µUŒ‚Œ ‚ğ‚Á‚Ä‚¢‚é‚È‚ç
	if (actor->CanAttack())
	{
		auto nextIt = it;
		++nextIt;
		//Ÿ‚ÌƒAƒNƒ^[‚É“n‚·
		if (nextIt != m_groupActors[groupName].end())
		{
			//UŒ‚Œ ‚ğ‚½‚¹‚é
			TakeTurnCanAttack(*nextIt, groupName);
		}
		else
		{
			//Ÿ‚ÌƒAƒNƒ^[‚ª‚¢‚È‚¢‚È‚çæ“ª‚É“n‚·
			TakeTurnCanAttack(m_groupActors[groupName].front(), groupName);
		}
	}
	//“o˜^‰ğœ
	m_groupActors[groupName].remove(actor);
}

void GroupManager::Init()
{
	m_groupActors = {};
	m_canAttackActorID = {};
	m_groupNames = {};
}

void GroupManager::Update()
{
	//“o˜^‚³‚ê‚½ƒOƒ‹[ƒv–¼‚©‚çŠeƒOƒ‹[ƒv‚ğŒ©‚é
	for (auto& groupName : m_groupNames)
	{
		//UŒ‚Œ ‚ğŸ‚É‰ñ‚·‚©‚Ç‚¤‚©
		bool isTurnCanAttack = false;
		//‰ñ‚¹‚½‚©
		bool isTurnedCanAttack = false;
		//ƒOƒ‹[ƒv“à‚ÅUŒ‚Œ ‚ğ‰ñ‚·
		for (auto& actor : m_groupActors[groupName])
		{
#if _DEBUG
			//‘«Œ³‚É‹…‘Ì‚ğo‚µ‚ÄF‚©‚ç‚Ç‚ÌƒOƒ‹[ƒv‚ÉŠ‘®‚µ‚Ä‚¢‚é‚©‚í‚©‚é‚æ‚¤‚É‚·‚é
			DrawSphere3D(actor->GetPos().ToDxLibVector(), 40.0f, 16, m_groupColor[groupName], m_groupColor[groupName], true);
#endif

			//‰ñ‚³‚È‚¢
			if (!isTurnCanAttack)
			{
				//ID‚©‚çUŒ‚Œ Š—LÒ‚©‚ÂUŒ‚Œ ‚ª‚È‚­‚È‚Á‚Ä‚¢‚é‚È‚ç
				if (actor->GetID() == m_canAttackActorID[groupName] && !actor->CanAttack())
				{
					//Ÿ‚ÌƒAƒNƒ^[‚É“n‚·
					isTurnCanAttack = true;
				}
			}
			//‰ñ‚·
			else
			{
				//“n‚¹‚½
				isTurnedCanAttack = true;
				TakeTurnCanAttack(actor, groupName);
				break;
			}
		}
		//“n‚·‚Â‚à‚è‚¾‚Á‚½‚ªŸ‚ÌƒAƒNƒ^[‚ª‚¢‚È‚­‚Ä“n‚¹‚È‚©‚Á‚½‚Æ‚«
		if (isTurnCanAttack && !isTurnedCanAttack)
		{
			//æ“ª‚É“n‚·
			TakeTurnCanAttack(m_groupActors[groupName].front(), groupName);
		}
	}
}

void GroupManager::End()
{
	//ƒAƒNƒ^[‚Ì“o˜^‰ğœ
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (auto& groupName : m_groupNames)
	{
		for (auto& actor : m_groupActors[groupName])
		{
			deleteActer.emplace_back(actor);
		}
	}
	for (auto& actor : deleteActer)
	{
		Exit(actor);
	}
	deleteActer.clear();
	//‚·‚×‚Ä“o˜^‰ğœ
	m_groupActors.clear();
	m_canAttackActorID.clear();
}

void GroupManager::TakeTurnCanAttack(std::shared_ptr<Actor> actor, std::string& groupName)
{
	//UŒ‚Œ ‚ğ‚½‚¹‚é
	actor->SetCanAttack(true);
	//ID“o˜^
	m_canAttackActorID[groupName] = actor->GetID();
}

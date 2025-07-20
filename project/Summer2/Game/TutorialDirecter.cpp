#include "TutorialDirecter.h"
#include "Actors/ActorManager.h"

TutorialDirecter::TutorialDirecter()
{
}

TutorialDirecter::~TutorialDirecter()
{
}
void TutorialDirecter::Init()
{

}
void TutorialDirecter::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーが存在しているかのチェック
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer();
}
void TutorialDirecter::End()
{

}
#pragma once
#include <memory>
class ActorManager;
/// <summary>
/// チュートリアルの進行をするクラス
/// </summary>
class TutorialDirecter
{
public:
	TutorialDirecter();
	~TutorialDirecter();
	void Init();
	void Update(const std::weak_ptr<ActorManager> actorManager);
	void End();
private:
	
};


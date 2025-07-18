#pragma once
#include <memory>

enum class UIData
{
	PlayerHp,
	PlayerUlt,
	EnemyHp,
	BossHp,
	Timer,
	Score
};

class ActorManager;
class UIBase abstract
{
public:
	UIBase(UIData uiData, int handle);
	virtual~UIBase();
	virtual void Init() abstract;
	virtual void Update(const std::weak_ptr<ActorManager> actorManager)abstract;
	virtual void Draw()const abstract;
	virtual void End()abstract;
	bool IsDelete() { return m_isDelete; };
	UIData GetUIData() { return m_uiData; };
protected:
	bool m_isDelete;	//íœ
	bool m_isDraw;		//•`‰æ‚·‚é‚©
	//UI‚Ì¯•Ê—p•Ï”
	UIData m_uiData;
	//ƒnƒ“ƒhƒ‹
	int m_handle;
};


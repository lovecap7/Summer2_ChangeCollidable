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
	bool m_isDelete;	//削除
	bool m_isDraw;		//描画するか
	//UIの識別用変数
	UIData m_uiData;
	//ハンドル
	int m_handle;
};


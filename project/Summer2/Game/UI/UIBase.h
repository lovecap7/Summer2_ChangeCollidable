#pragma once
#include <memory>
class ActorManager;
class UIBase abstract
{
public:
	UIBase();
	virtual~UIBase();
	virtual void Init() abstract;
	virtual void Update(const std::weak_ptr<ActorManager> actorManager)abstract;
	virtual void Draw()const abstract;
	virtual void End()abstract;
	bool IsDelete() { return m_isDelete; };
protected:
	bool m_isDelete;	//�폜
	bool m_isDraw;		//�`�悷�邩
};


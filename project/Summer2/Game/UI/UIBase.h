#pragma once
class UIBase abstract
{
public:
	UIBase();
	virtual~UIBase();
	virtual void Init() abstract;
	virtual void Update()abstract;
	virtual void Draw()const abstract;
	virtual void End()abstract;
	bool IsDelete() { return m_isDelete; };
protected:
	bool m_isDelete;
};


#pragma once
#include <memory>

class UIBase abstract :
	public std::enable_shared_from_this<UIBase>
{
public:
	UIBase();
	virtual~UIBase();
	virtual void Init() ;
	virtual void Update()abstract;
	virtual void Draw()const abstract;
	virtual void End();
	bool IsDelete() { return m_isDelete; };
	//ï`âÊÇ∑ÇÈÇ©
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; };
	bool IsDraw()const { return m_isDraw; };
protected:
	bool m_isDelete;	//çÌèú
	bool m_isDraw;		//ï`âÊÇ∑ÇÈÇ©
};


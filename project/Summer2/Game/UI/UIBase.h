#pragma once
#include <memory>

class UIBase abstract :
	public std::enable_shared_from_this<UIBase>
{
public:
	UIBase(int handle);
	virtual~UIBase();
	virtual void Init() ;
	virtual void Update()abstract;
	virtual void Draw()const abstract;
	virtual void End();
	bool IsDelete() { return m_isDelete; };
	//•`‰æ‚·‚é‚©
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; };
	bool IsDraw()const { return m_isDraw; };
protected:
	bool m_isDelete;	//íœ
	bool m_isDraw;		//•`‰æ‚·‚é‚©
	//ƒnƒ“ƒhƒ‹
	int m_handle;
};


#pragma once
#include <memory>

class UIBase abstract :
	public std::enable_shared_from_this<UIBase>
{
public:
	UIBase(bool isFront = true);
	virtual~UIBase();
	virtual void Init() ;
	virtual void Update()abstract;
	virtual void Draw()const abstract;
	virtual void End();
	bool IsDelete() { return m_isDelete; };
	void Delete() { m_isDelete = true; };
	//•`‰æ‚·‚é‚©
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; };
	bool IsDraw()const { return m_isDraw; };
	//•`‰æ‚ğ‘O‚É‚·‚é
	bool IsFront()const { return m_isFront; };
protected:
	bool m_isDelete;	//íœ
	bool m_isDraw;		//•`‰æ‚·‚é‚©
	//‘O‚É•`‰æ‚·‚é
	bool m_isFront;
};


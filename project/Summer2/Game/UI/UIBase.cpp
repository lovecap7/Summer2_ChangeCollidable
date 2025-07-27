#include "UIBase.h"
#include "UIManager.h"

UIBase::UIBase(bool isFront):
	m_isDelete(false),
	m_isDraw(true),
	m_isFront(isFront)
{
}

UIBase::~UIBase()
{
}

void UIBase::Init()
{
	//Physics‚É“o˜^
	UIManager::GetInstance().Entry(shared_from_this());
}

void UIBase::End()
{
	//Physics‚©‚ç“o˜^‰ğœ
	UIManager::GetInstance().Exit(shared_from_this());
}
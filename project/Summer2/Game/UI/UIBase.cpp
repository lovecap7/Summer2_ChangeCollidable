#include "UIBase.h"

UIBase::UIBase(UIData uiData,int handle):
	m_isDelete(false),
	m_isDraw(true),
	m_uiData(uiData),
	m_handle(handle)
{
}

UIBase::~UIBase()
{
}

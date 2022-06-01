#include "stdafx.h"
#include "UIChildWindow.h"

namespace DuiLib
{

class CChildWnd : public CWindowWnd
{
public:

#ifdef __MINGW32__
    using __super = CWindowWnd;
#endif
	CChildWnd(): m_pOwner(NULL)
	{

	}

	void Init(CChildWindowUI* pOwner)
	{
		m_pOwner = pOwner;
		UINT uStyle = WS_CHILD | WS_VISIBLE;
		Create(m_pOwner->GetManager()->GetPaintWindow(), _T(""), uStyle, 0, 0, 0, 0, 0);
	}

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("ChildWindow");
	}

	void OnFinalMessage(HWND hWnd)
	{
		m_pOwner->m_pWindow = NULL;
		__super::OnFinalMessage(hWnd);
		delete this;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = FALSE;

		if( uMsg == WM_CREATE )
		{
			m_pm.Init(GetHWND());
		}
		else if( uMsg == WM_PAINT)
		{
			RECT rc = m_pOwner->GetPos();
			PAINTSTRUCT ps = { 0 };
			::BeginPaint(m_hWnd, &ps);

			RECT rcPaint;
			::GetClientRect(m_hWnd, &rcPaint);

			m_pm.Render()->Resize(rcPaint);
			if(m_pOwner->GetBkColor() == 0)
				m_pm.Render()->DrawColor(ps.rcPaint, CDuiSize(0,0), 0xFF000000);
			else
				m_pm.Render()->DrawColor(ps.rcPaint, CDuiSize(0,0), m_pOwner->GetBkColor());
			::BitBlt(ps.hdc, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, m_pm.Render()->GetDC(), rcPaint.left, rcPaint.top, SRCCOPY);

			::EndPaint(m_hWnd, &ps);
		}
		else if( uMsg == WM_SIZE)
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_WINDOWSIZE, 0, 0);
		}
		else if( uMsg == WM_CLOSE)
		{
			::DestroyWindow(m_hWnd);
			bHandled = TRUE;
		}

		if( !bHandled ) 
		{
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}
		return lRes;
	}

	CPaintManagerUI m_pm;
private:
	CChildWindowUI *m_pOwner;
};

///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CChildWindowUI)

CChildWindowUI::CChildWindowUI(void) : m_pWindow(NULL)
{

}


CChildWindowUI::~CChildWindowUI(void)
{	
	//DestroyWnd();
}

void CChildWindowUI::DoInit()
{
	CreateWnd();
}

void CChildWindowUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	SetInternVisible(bVisible);
}

void CChildWindowUI::SetInternVisible(bool bVisible)
{
	CControlUI::SetInternVisible(bVisible);
	if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
	{
		if(bVisible)
			::ShowWindow(m_pWindow->GetHWND(), SW_SHOW);
		else
			::ShowWindow(m_pWindow->GetHWND(), SW_HIDE);
	}
}

LPCTSTR CChildWindowUI::GetClass() const
{
	return _T("ChildWindowUI");
}

LPVOID CChildWindowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_CHILDWINDOW) == 0 ) return static_cast<CChildWindowUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CChildWindowUI::DoEvent(TEventUI& event)
{
	__super::DoEvent(event);
}

void CChildWindowUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);

	if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
	{
		::MoveWindow(m_pWindow->GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
	}
}

void CChildWindowUI::Move(SIZE szOffset, bool bNeedInvalidate)
{
	__super::Move(szOffset, bNeedInvalidate);

	if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
	{
		RECT rc = GetPos();
		::MoveWindow(m_pWindow->GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
	}
}

void CChildWindowUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
// 	if (_tcscmp(pstrName, _T("homepage")) == 0)
// 	{
// 		m_strURL = pstrValue;
// 	}
// 	else 
		__super::SetAttribute(pstrName,pstrValue);
}


CWindowWnd *CChildWindowUI::CreateWnd()
{
	if(m_pWindow != NULL)
		return m_pWindow;

	m_pWindow = new CChildWnd();
	((CChildWnd *)m_pWindow)->Init(this);
	return m_pWindow;
}

void CChildWindowUI::DestroyWnd()
{
	if(m_pWindow)
	{
		m_pWindow->Close();
		m_pWindow = NULL;
	}
}

CWindowWnd *CChildWindowUI::GetWnd()
{
	return m_pWindow;
}

void CChildWindowUI::RefreshWindow()
{
	if(m_pWindow)
		::InvalidateRect(m_pWindow->GetHWND(), NULL, TRUE);
}

}

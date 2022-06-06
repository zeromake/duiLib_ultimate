#include "stdafx.h"
#include "UIHorizontalLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CHorizontalLayoutUI)
	CHorizontalLayoutUI::CHorizontalLayoutUI() : m_iSepWidth(0), m_bImmMode(false)
	{
		::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
	}

	LPCTSTR CHorizontalLayoutUI::GetClass() const
	{
		return _T("HorizontalLayoutUI");
	}

	LPVOID CHorizontalLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_HORIZONTALLAYOUT) == 0 ) return static_cast<CHorizontalLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CHorizontalLayoutUI::GetControlFlags() const
	{
		if( IsEnabled() && m_iSepWidth != 0 ) return UIFLAG_SETCURSOR;
		else return 0;
	}

	SIZE CHorizontalLayoutUI::EstimateSize(SIZE szAvailable)
	{
		if (IsAnimationRunning(ANIMATION_ID_SHOW) || IsAnimationRunning(ANIMATION_ID_HIDE)) {
			return m_szAnimationCurrect;
		}

		if (!IsPaneVisible())
		{
			return m_szAnimationCurrect;
		}

		if (IsAutoCalcWidth() || IsAutoCalcHeight())
		{
			SIZE sz = { 0 };
			for (int it = 0; it < GetCount(); it++)
			{
				SIZE szControl = { 0 };
				CControlUI* pControl = GetItemAt(it);
				if (!pControl->IsVisible()) continue;
				szControl = pControl->EstimateSize(szAvailable);
				RECT padding = pControl->GetPadding();
				if (IsAutoCalcWidth())
				{
					sz.cx += szControl.cx;
					sz.cx += padding.left + padding.right;
				}

				if (IsAutoCalcHeight())
				{
					if (sz.cy < szControl.cy + padding.top + padding.bottom)
					{
						sz.cy = szControl.cy + padding.top + padding.bottom;
					}
				}
			}

			RECT rcInset = GetInset();
			if (IsAutoCalcWidth())
			{
				sz.cx += rcInset.left + rcInset.right;
				if (GetCount() > 1) sz.cx += (GetCount() - 1) * m_iChildPadding;
			}
			if (IsAutoCalcHeight())
			{
				sz.cy += rcInset.top + rcInset.bottom;
			}

			return sz;
		}
		return CControlUI::EstimateSize(szAvailable);
	}

	void CHorizontalLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		// Adjust for inset
		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

		if( m_items.GetSize() == 0) {
			ProcessScrollBar(rc, 0, 0);
			return;
		}

		// Determine the minimum size
		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		//if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		//	szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
			szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();

		//第一次循环，计算哪些控件需要自动布局，以及剩余可自动布局的空间。

		int cyNeeded = 0;
		int nAdjustables = 0;	//需要自动布局的控件数量
		int cxFixed = 0;
		int nEstimateNum = 0;
		SIZE szControlAvailable;
		int iControlMaxWidth = 0;
		int iControlMaxHeight = 0;
		for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) continue;
			szControlAvailable = szAvailable;
			RECT rcPadding = pControl->GetPadding();
			szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if(sz.cx == 0 && pControl->GetFixedWidthPercent() > 0)
			{
				sz.cx = szAvailable.cx * pControl->GetFixedWidthPercent() / 100;
			}

			if( sz.cx == 0 ) //控件的宽度为0，需要自动布局
			{ 
				nAdjustables++;			
			}
			else {
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			}
			cxFixed += sz.cx + pControl->GetPadding().left + pControl->GetPadding().right;

			sz.cy = MAX(sz.cy, 0L);
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			cyNeeded = MAX((LONG)cyNeeded, sz.cy + rcPadding.top + rcPadding.bottom);
			nEstimateNum++;
		}
		cxFixed += (nEstimateNum - 1) * m_iChildPadding;
		// Place elements
		int cxNeeded = 0;
		int cxExpand = 0;	//需要自动调整控件的宽度，也就是 width==0 的子控件
		if( nAdjustables > 0 ) cxExpand = MAX(0L, (szAvailable.cx - cxFixed) / nAdjustables);
		// Position the elements
		SIZE szRemaining = szAvailable;
		int iPosX = rc.left;
		
		// 滚动条
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
			iPosX -= m_pHorizontalScrollBar->GetScrollPos();
		}
		else {
			// 子控件横向对其方式
			if(nAdjustables <= 0) {
				UINT iChildAlign = GetChildAlign(); 
				if (iChildAlign == DT_CENTER) {
					iPosX += (szAvailable.cx -cxFixed) / 2;
				}
				else if (iChildAlign == DT_RIGHT) {
					iPosX += (szAvailable.cx - cxFixed);
				}
			}
		}

		//第二次循环，剩余可自动布局的空间均分给需要自动布局的子控件。

		int iEstimate = 0;
		int iAdjustable = 0;
		int cxFixedRemaining = cxFixed;
		for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) {
				SetFloatPos(it2);
				continue;
			}
			
			iEstimate += 1;
			RECT rcPadding = pControl->GetPadding();
			szRemaining.cx -= rcPadding.left;

			szControlAvailable = szRemaining;
			szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			cxFixedRemaining = cxFixedRemaining - (rcPadding.left + rcPadding.right);
			if (iEstimate > 1) cxFixedRemaining = cxFixedRemaining - m_iChildPadding;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if(sz.cx == 0 && pControl->GetFixedWidthPercent() > 0)
			{
				sz.cx = szAvailable.cx * pControl->GetFixedWidthPercent() / 100;
			}

			if( sz.cx == 0 ) {
				iAdjustable++;
				sz.cx = cxExpand;
				// Distribute remaining to last element (usually round-off left-overs)
				if( iAdjustable == nAdjustables ) {
					sz.cx = MAX(0L, szRemaining.cx - rcPadding.right - cxFixedRemaining);
				} 
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			}
			else {
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
				cxFixedRemaining -= sz.cx;
			}

// 			sz.cy = pControl->GetMaxHeight();
// 			if( sz.cy == 0 ) sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
// 			if( sz.cy < 0 ) sz.cy = 0;
// 			if( sz.cy > szControlAvailable.cy ) sz.cy = szControlAvailable.cy;
// 			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if(sz.cy == 0 && pControl->GetFixedHeight() == 0 && pControl->GetFixedHeightPercent() > 0) 
				sz.cy = szAvailable.cy * pControl->GetFixedHeightPercent() / 100;
			if( sz.cy == 0 ) sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
			if( sz.cy < 0 ) sz.cy = 0;
			if( sz.cy > szControlAvailable.cy ) sz.cy = szControlAvailable.cy;
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();

			UINT iChildAlign = GetChildVAlign(); 
			if (iChildAlign == DT_VCENTER) {
				int iPosY = (rc.bottom + rc.top) / 2;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
					iPosY += m_pVerticalScrollBar->GetScrollRange() / 2;
					iPosY -= m_pVerticalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY - sz.cy/2, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy - sz.cy/2 };
				pControl->SetPos(rcCtrl, false);
			}
			else if (iChildAlign == DT_BOTTOM) {
				int iPosY = rc.bottom;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
					iPosY += m_pVerticalScrollBar->GetScrollRange();
					iPosY -= m_pVerticalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY - rcPadding.bottom - sz.cy, iPosX + sz.cx + rcPadding.left, iPosY - rcPadding.bottom };
				pControl->SetPos(rcCtrl, false);
			}
			else {
				int iPosY = rc.top;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
					iPosY -= m_pVerticalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}

			iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
			cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
			szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
		}
		cxNeeded += (nEstimateNum - 1) * m_iChildPadding;

		// Process the scrollbar
		ProcessScrollBar(rc, cxNeeded, cyNeeded);
	}

	void CHorizontalLayoutUI::DoPostPaint(UIRender *pRender, const RECT& rcPaint)
	{
		if( IsCaptureState() && !m_bImmMode ) {
			RECT rcSeparator = GetThumbRect(true);
			pRender->DrawColor(rcSeparator, CDuiSize(0,0), 0xAA000000);
		}
	}

	void CHorizontalLayoutUI::SetSepWidth(int iWidth)
	{
		m_iSepWidth = iWidth;
	}

	int CHorizontalLayoutUI::GetSepWidth() const
	{
		return m_iSepWidth;
	}

	void CHorizontalLayoutUI::SetSepImmMode(bool bImmediately)
	{
		if( m_bImmMode == bImmediately ) return;
		if( IsCaptureState() && !m_bImmMode && m_pManager != NULL ) {
			m_pManager->RemovePostPaint(this);
		}

		m_bImmMode = bImmediately;
	}

	bool CHorizontalLayoutUI::IsSepImmMode() const
	{
		return m_bImmMode;
	}

	void CHorizontalLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("sepwidth")) == 0 ) SetSepWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("sepimm")) == 0 ) SetSepImmMode(_tcsicmp(pstrValue, _T("true")) == 0);
		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CHorizontalLayoutUI::DoEvent(TEventUI& event)
	{
		if( m_iSepWidth != 0 ) {
			if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
			{
				RECT rcSeparator = GetThumbRect(false);
				if( ::PtInRect(&rcSeparator, event.ptMouse) ) {
					SetCaptureState(true);
					m_ptLastMouse = event.ptMouse;
					m_rcNewPos = m_rcItem;
					if( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint(this);
					return;
				}
			}
			if( event.Type == UIEVENT_BUTTONUP )
			{
				if( IsCaptureState() ) {
					SetCaptureState(false);
					m_rcItem = m_rcNewPos;
					if( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint(this);
					NeedParentUpdate();
					return;
				}
			}
			if( event.Type == UIEVENT_MOUSEMOVE )
			{
				if( IsCaptureState() ) {
					LONG cx = event.ptMouse.x - m_ptLastMouse.x;
					m_ptLastMouse = event.ptMouse;
					RECT rc = m_rcNewPos;
					if( m_iSepWidth >= 0 ) {
						if( cx > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepWidth ) return;
						if( cx < 0 && event.ptMouse.x > m_rcNewPos.right ) return;
						rc.right += cx;
						if( rc.right - rc.left <= GetMinWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
							rc.right = rc.left + GetMinWidth();
						}
						if( rc.right - rc.left >= GetMaxWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
							rc.right = rc.left + GetMaxWidth();
						}
					}
					else {
						if( cx > 0 && event.ptMouse.x < m_rcNewPos.left ) return;
						if( cx < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepWidth ) return;
						rc.left += cx;
						if( rc.right - rc.left <= GetMinWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
							rc.left = rc.right - GetMinWidth();
						}
						if( rc.right - rc.left >= GetMaxWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
							rc.left = rc.right - GetMaxWidth();
						}
					}

					CDuiRect rcInvalidate = GetThumbRect(true);
					m_rcNewPos = rc;
					m_cxyFixed.cx = m_rcNewPos.right - m_rcNewPos.left;

					if( m_bImmMode ) {
						m_rcItem = m_rcNewPos;
						NeedParentUpdate();
					}
					else {
						rcInvalidate.Join(GetThumbRect(true));
						rcInvalidate.Join(GetThumbRect(false));
						if( m_pManager ) m_pManager->Invalidate(rcInvalidate);
					}
					return;
				}
			}
			if( event.Type == UIEVENT_SETCURSOR )
			{
				RECT rcSeparator = GetThumbRect(false);
				if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) {
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
					return;
				}
			}
		}
		CContainerUI::DoEvent(event);
	}

	RECT CHorizontalLayoutUI::GetThumbRect(bool bUseNew) const
	{
		if( IsCaptureState() && bUseNew) {
			if( m_iSepWidth >= 0 ) return CDuiRect(m_rcNewPos.right - m_iSepWidth, m_rcNewPos.top, m_rcNewPos.right, m_rcNewPos.bottom);
			else return CDuiRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.left - m_iSepWidth, m_rcNewPos.bottom);
		}
		else {
			if( m_iSepWidth >= 0 ) return CDuiRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
			else return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
		}
	}

	bool CHorizontalLayoutUI::CalcPos(CControlUI *pChildControl, RECT &rcChild)
	{
		CDuiRect rc = m_rcItem;

		// Adjust for inset
		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

		if( m_items.GetSize() == 0)  return false;

		// Determine the minimum size
		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		//if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		//	szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
			szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();

		int cyNeeded = 0;
		int nAdjustables = 0;
		int cxFixed = 0;
		int nEstimateNum = 0;
		SIZE szControlAvailable;
		int iControlMaxWidth = 0;
		int iControlMaxHeight = 0;
		for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
			if( !pControl->IsVisible() && !pControl->IsPaneVisible() ) continue; //隐藏的控件也要计算的
			if( pControl->IsFloat() ) continue;
			szControlAvailable = szAvailable;
			RECT rcPadding = pControl->GetPadding();
			szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if(sz.cx == 0 && pControl->GetFixedWidthPercent() > 0)
			{
				sz.cx = szAvailable.cx * pControl->GetFixedWidthPercent() / 100;
			}
			if( sz.cx == 0 ) {
				nAdjustables++;
			}
			else {
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			}
			cxFixed += sz.cx + pControl->GetPadding().left + pControl->GetPadding().right;

			sz.cy = MAX(sz.cy, 0L);
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			cyNeeded = MAX((LONG)cyNeeded, sz.cy + rcPadding.top + rcPadding.bottom);
			nEstimateNum++;
		}
		cxFixed += (nEstimateNum - 1) * m_iChildPadding;
		// Place elements
		int cxNeeded = 0;
		int cxExpand = 0;
		if( nAdjustables > 0 ) cxExpand = MAX(0L, (szAvailable.cx - cxFixed) / nAdjustables);
		// Position the elements
		SIZE szRemaining = szAvailable;
		int iPosX = rc.left;

		// 滚动条
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
			iPosX -= m_pHorizontalScrollBar->GetScrollPos();
		}
		else {
			// 子控件横向对其方式
			if(nAdjustables <= 0) {
				UINT iChildAlign = GetChildAlign(); 
				if (iChildAlign == DT_CENTER) {
					iPosX += (szAvailable.cx -cxFixed) / 2;
				}
				else if (iChildAlign == DT_RIGHT) {
					iPosX += (szAvailable.cx - cxFixed);
				}
			}
		}
		int iEstimate = 0;
		int iAdjustable = 0;
		int cxFixedRemaining = cxFixed;
		for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
			if( !pControl->IsVisible() && !pControl->IsPaneVisible() ) continue; //隐藏的控件也要计算的
			if( pControl->IsFloat() ) {
				if(pControl == pChildControl) {
					rcChild = pChildControl->GetPos();
				}
				continue;
			}

			iEstimate += 1;
			RECT rcPadding = pControl->GetPadding();
			szRemaining.cx -= rcPadding.left;

			szControlAvailable = szRemaining;
			szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			cxFixedRemaining = cxFixedRemaining - (rcPadding.left + rcPadding.right);
			if (iEstimate > 1) cxFixedRemaining = cxFixedRemaining - m_iChildPadding;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if(sz.cx == 0 && pControl->GetFixedWidthPercent() > 0)
			{
				sz.cx = szAvailable.cx * pControl->GetFixedWidthPercent() / 100;
			}
			if( sz.cx == 0 ) {
				iAdjustable++;
				sz.cx = cxExpand;
				// Distribute remaining to last element (usually round-off left-overs)
				if( iAdjustable == nAdjustables ) {
					sz.cx = MAX(0L, szRemaining.cx - rcPadding.right - cxFixedRemaining);
				} 
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			}
			else {
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
				cxFixedRemaining -= sz.cx;
			}

			if(sz.cy == 0 && pControl->GetFixedHeight() == 0 && pControl->GetFixedHeightPercent() > 0) 
				sz.cy = szAvailable.cy * pControl->GetFixedHeightPercent() / 100;
			if( sz.cy == 0 ) sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom;
			if( sz.cy < 0 ) sz.cy = 0;
			if( sz.cy > szControlAvailable.cy ) sz.cy = szControlAvailable.cy;
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();

			UINT iChildAlign = GetChildVAlign(); 
			if (iChildAlign == DT_VCENTER) {
				int iPosY = (rc.bottom + rc.top) / 2;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
					iPosY += m_pVerticalScrollBar->GetScrollRange() / 2;
					iPosY -= m_pVerticalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY - sz.cy/2, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy - sz.cy/2 };
				//pControl->SetPos(rcCtrl, false);
				if(pControl == pChildControl) { rcChild = rcCtrl; return true; }
			}
			else if (iChildAlign == DT_BOTTOM) {
				int iPosY = rc.bottom;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
					iPosY += m_pVerticalScrollBar->GetScrollRange();
					iPosY -= m_pVerticalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY - rcPadding.bottom - sz.cy, iPosX + sz.cx + rcPadding.left, iPosY - rcPadding.bottom };
				//pControl->SetPos(rcCtrl, false);
				if(pControl == pChildControl) { rcChild = rcCtrl; return true; }
			}
			else {
				int iPosY = rc.top;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
					iPosY -= m_pVerticalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy + rcPadding.top };
				//pControl->SetPos(rcCtrl, false);
				if(pControl == pChildControl) { rcChild = rcCtrl; return true; }
			}

			iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
			cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
			szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
		}
		cxNeeded += (nEstimateNum - 1) * m_iChildPadding;
		return false;
	}
}

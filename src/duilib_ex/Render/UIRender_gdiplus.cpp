#include "StdAfx.h"
#include "UIRender_gdiplus.h"

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender_gdiplus::UIRender_gdiplus()
	{
		
	}

	UIRender_gdiplus::~UIRender_gdiplus()
	{}

	void UIRender_gdiplus::DrawColor(const RECT& rc, const SIZE &round, DWORD color)
	{
		if( color <= 0x00FFFFFF ) return;

		if(round.cx == 0 && round.cy == 0)
		{
			Gdiplus::Graphics graphics(m_hDC);
			Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
			graphics.FillRectangle(&brush, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));
		}
		else
		{
			Gdiplus::Graphics graphics(m_hDC);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			//graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
			Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
			Gdiplus::GraphicsPath pPath;

			int x = rc.left;
			int y = rc.top;
			int width = rc.right- rc.left; //不需要修正区域
			int height = rc.bottom-rc.top; //不需要修正区域

			int roundX = round.cx;
			int roundY = round.cy;
			// 圆角矩形的半宽和半高
			if(roundX > width/2) roundX = width/2;
			if(roundY > height/2) roundY = height/2;

			//左上圆角
			pPath.AddArc(x, y, 2*roundX, 2*roundY, 180, 90); //圆角不乘以2就有锯齿？
			//顶部横线
			pPath.AddLine(x+roundX, y, x+width-roundX, y);
			//右上圆角
			pPath.AddArc(x+width-2*roundX, y, 2*roundX, 2*roundY, 270, 90);
			//右侧竖线
			pPath.AddLine(x+width, y+roundY, x+width, y+height-roundY);
			//右下圆角
			pPath.AddArc(x+width-2*roundX, y+height-2*roundY, 2*roundX, 2*roundY, 0, 90);
			//底部横线
			pPath.AddLine(x+width-roundX, y+height, x+roundX, y+height);
			//左下圆角
			pPath.AddArc(x, y+height-2*roundY, 2*roundX, 2*roundY, 90, 90);
			//左侧横线
			pPath.AddLine(x, y+height-roundY, x, y+roundY);
			pPath.CloseFigure();

			graphics.FillPath(&brush, &pPath);
		}
	}

// 	void UIRender_gdiplus::DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps)
// 	{
// 		Gdiplus::Graphics graphics(m_hDC);
// 		Gdiplus::LinearGradientBrush linGrBrush(Gdiplus::PointF(rc.left,rc.top), Gdiplus::PointF(rc.top,rc.bottom), Gdiplus::Color(255,255,0,0), Gdiplus::Color(255,0,0,255));
// 		graphics.FillRectangle(&linGrBrush, rc.left, rc.top, rc.right, rc.bottom);
// 	}

	void UIRender_gdiplus::DrawLine(int x1, int y1, int x2, int y2, int nSize, DWORD dwPenColor,int nStyle)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		switch(nStyle)
		{
		case PS_SOLID :								// 0
			pen.SetDashStyle(Gdiplus::DashStyleSolid);
			break;
		case PS_DASH :								// 1       /* -------  */
			pen.SetDashStyle(Gdiplus::DashStyleDash);
			break;
		case PS_DOT :								// 2       /* .......  */
			pen.SetDashStyle(Gdiplus::DashStyleDot);
			break;
		case PS_DASHDOT :							// 3       /* _._._._  */
			pen.SetDashStyle(Gdiplus::DashStyleDashDot);
			break;
		case PS_DASHDOTDOT :						// 4       /* _.._.._  */
			pen.SetDashStyle(Gdiplus::DashStyleDashDotDot);
			break;
		}
		graphics.DrawLine(&pen, x1, y1, x2, y2);
	}

	void UIRender_gdiplus::DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle/* = PS_SOLID*/)
	{
		Gdiplus::Graphics graphics(m_hDC);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		pen.SetAlignment(Gdiplus::PenAlignmentInset);
		graphics.DrawRectangle(&pen, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left - 1), (INT)(rc.bottom - rc.top - 1));
	}

	void UIRender_gdiplus::DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		//graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		Gdiplus::GraphicsPath pPath;
		
		int x = rc.left;
		int y = rc.top;
		int width = rc.right- rc.left - nSize; //修正区域
		int height = rc.bottom-rc.top - nSize; //修正区域

		int roundX = round.cx;
		int roundY = round.cy;
		// 圆角矩形的半宽和半高
		if(roundX > width/2) roundX = width/2;
		if(roundY > height/2) roundY = height/2;

		//左上圆角
		pPath.AddArc(x, y, 2*roundX, 2*roundY, 180, 90); //圆角不乘以2就有锯齿？
		//顶部横线
		pPath.AddLine(x+roundX, y, x+width-roundX, y);
		//右上圆角
		pPath.AddArc(x+width-2*roundX, y, 2*roundX, 2*roundY, 270, 90);
		//右侧竖线
		pPath.AddLine(x+width, y+roundY, x+width, y+height-roundY);
		//右下圆角
		pPath.AddArc(x+width-2*roundX, y+height-2*roundY, 2*roundX, 2*roundY, 0, 90);
		//底部横线
		pPath.AddLine(x+width-roundX, y+height, x+roundX, y+height);
		//左下圆角
		pPath.AddArc(x, y+height-2*roundY, 2*roundX, 2*roundY, 90, 90);
		//左侧横线
		pPath.AddLine(x, y+height-roundY, x, y+roundY);
		pPath.CloseFigure();

		graphics.DrawPath(&pen, &pPath);
	}

	void UIRender_gdiplus::DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen pen(dwPenColor, (Gdiplus::REAL)nSize);
		graphics.DrawEllipse(&pen, (INT)rc.left, (INT)rc.top, (INT)(rc.right-rc.left), (INT)(rc.bottom-rc.top));
	}

	void UIRender_gdiplus::FillEllipse(const RECT& rc, DWORD dwColor)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::SolidBrush brush(dwColor);
		graphics.FillEllipse(&brush, (INT)rc.left, (INT)rc.top, (INT)(rc.right-rc.left), (INT)(rc.bottom-rc.top));
	}

	void UIRender_gdiplus::DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle)
	{
		CPaintManagerUI *pManager = GetManager();
		HDC hDC = GetDC();

		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
		if( pstrText == NULL || pManager == NULL ) return;

		CStdRefPtr<UIObject> pOldFont = SelectObject(pManager->GetFont(iFont));

		Gdiplus::Font font(hDC, (HFONT)pManager->GetFont(iFont)->GetHFont(GetManager()));

		Gdiplus::Graphics graphics( hDC );
		Gdiplus::TextRenderingHint trh = Gdiplus::TextRenderingHintSystemDefault;
		switch(pManager->GetGdiplusTextRenderingHint()) 
		{
		case 0: {trh = Gdiplus::TextRenderingHintSystemDefault; break;}
		case 1: {trh = Gdiplus::TextRenderingHintSingleBitPerPixelGridFit; break;}
		case 2: {trh = Gdiplus::TextRenderingHintSingleBitPerPixel; break;}
		case 3: {trh = Gdiplus::TextRenderingHintAntiAliasGridFit; break;}
		case 4: {trh = Gdiplus::TextRenderingHintAntiAlias; break;}
		case 5: {trh = Gdiplus::TextRenderingHintClearTypeGridFit; break;}
		}
		graphics.SetTextRenderingHint(trh);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality); 
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

		Gdiplus::RectF rectF((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, (Gdiplus::REAL)(rc.right - rc.left), (Gdiplus::REAL)(rc.bottom - rc.top));
		Gdiplus::SolidBrush brush(Gdiplus::Color(254, GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

		Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();

		if ((uStyle & DT_END_ELLIPSIS) != 0) {
			stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
		}

		int formatFlags = 0;
		if ((uStyle & DT_NOCLIP) != 0) {
			formatFlags |= Gdiplus::StringFormatFlagsNoClip;
		}
		if ((uStyle & DT_SINGLELINE) != 0) {
			formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
		}

		stringFormat.SetFormatFlags(formatFlags);

		if ((uStyle & DT_LEFT) != 0) {
			stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
		}
		else if ((uStyle & DT_CENTER) != 0) {
			stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		}
		else if ((uStyle & DT_RIGHT) != 0) {
			stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
		}
		else {
			stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
		}
		stringFormat.GenericTypographic();
		if ((uStyle & DT_TOP) != 0) {
			stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
		}
		else if ((uStyle & DT_VCENTER) != 0) {
			stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		}
		else if ((uStyle & DT_BOTTOM) != 0) {
			stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
		}
		else {
			stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
		}
#ifdef UNICODE
		if ((uStyle & DT_CALCRECT) == DT_CALCRECT)
		{
			Gdiplus::RectF bounds;

			//UIFont_gdiplus *pFont = *(UIFont_gdiplus **)&m_curFont;
			//graphics.MeasureString(pstrText, -1, pFont->GetGdiPlusFont(), rectFCalc, &stringFormat, &bounds);
			graphics.MeasureString(pstrText, -1, &font, rectF, &stringFormat, &bounds);

			// MeasureString存在计算误差，这里加一像素
			rc.bottom = rc.top + (long)bounds.Height + 1;
			rc.right = rc.left + (long)bounds.Width + 1;
		}
		else
		{
			//UIFont_gdiplus *pFont = *(UIFont_gdiplus **)&m_curFont;
			//graphics.DrawString(pstrText, -1, pFont->GetGdiPlusFont(), rectF, &stringFormat, &brush);
			graphics.DrawString(pstrText, -1, &font, rectF, &stringFormat, &brush);
		}
#else
		DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, pstrText, -1, NULL, 0);
		WCHAR * pcwszDest = new WCHAR[dwSize + 1];
		memset(pcwszDest, 0, (dwSize + 1) * sizeof(WCHAR));
		MultiByteToWideChar(CP_ACP, 0, pstrText, -1, pcwszDest, dwSize);
		if(pcwszDest != NULL)
		{
			if ((uStyle & DT_CALCRECT) == DT_CALCRECT)
			{
				Gdiplus::RectF bounds;
				graphics.MeasureString(pcwszDest, -1, &font, rectF, &stringFormat, &bounds);
				rc.bottom = rc.top + (long)(bounds.Height * 1.06);
				rc.right = rc.left + (long)(bounds.Width * 1.06);
			}
			else
			{
				graphics.DrawString(pcwszDest, -1, &font, rectF, &stringFormat, &brush);
			}
			delete []pcwszDest;
		}
#endif
		SelectObject(pOldFont);
	}

	UIPath* UIRender_gdiplus::CreatePath()
	{
		return new UIPath_gdiplus();
	}

	BOOL UIRender_gdiplus::DrawPath(const UIPath* path, int nSize, DWORD dwColor)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen pen(dwColor, (Gdiplus::REAL)nSize);
		return graphics.DrawPath(&pen, ((UIPath_gdiplus *)path)->Getpath()) == Gdiplus::Ok;
	}

	BOOL UIRender_gdiplus::FillPath(const UIPath* path, const DWORD dwColor)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::SolidBrush brush(dwColor);
		return graphics.FillPath(&brush, ((UIPath_gdiplus *)path)->Getpath()) == Gdiplus::Ok;
	}
} // namespace DuiLib

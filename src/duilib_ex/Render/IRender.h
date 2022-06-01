#ifndef __IRENDER_H__
#define __IRENDER_H__

#pragma once

#include <wingdi.h>

#ifndef UILIB_API
#define UILIB_API
#endif

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
    class UIRect: public tagRECT {
        public:
        // uninitialized rectangle
        UIRect() throw();
        // from left, top, right, and bottom
        UIRect(
            int l,
            int t,
            int r,
            int b) throw();
        // copy constructor
        UIRect(const RECT& srcRect) throw();

        // retrieves the width
        int Width() const throw();
        // returns the height
        int Height() const throw();
	    void operator=(const RECT& srcRect) throw();
        operator LPRECT() throw();
        operator LPCRECT() const throw();
        bool IsRectEmpty() const throw();
        bool IsRectNull() const throw();
    };
    class CPaintManagerUI;
	typedef struct UILIB_API tagTDrawInfo
	{
		tagTDrawInfo();
		void Parse(LPCTSTR pStrImage, LPCTSTR pStrModify, CPaintManagerUI *paintManager);
		void Clear();

		CDuiString sDrawString;	//xml中定义的bkimage属性等等，如：file='images\background.png' dest='0,0,357,220' 
		CDuiString sDrawModify; //如果不为空， sDrawString + sDrawModify 为标识关键字

		//------以下都是从sDrawString解析出来的----------//

		CDuiString sImageName;	//图片的文件名，这玩意会给UIImageInfo列表做关键字，好处是同一张图片，内存中只保存一份。 (svg除外, 同一张svg渲染不同时，内存中保存多份。)
		CDuiString sResType;	//资源文件中定义的类型，如："ZIPRES", "PNG"
		RECT rcDest;			//显示在什么位置
		RECT rcSource;			//需要用图片的哪部分？
		RECT rcCorner;			//圆角
		DWORD dwMask;			//位图的透明色
		BYTE uFade;				//透明度
		bool bHole;				//洞洞.......rcCorner区域不绘制
		bool bTiledX;			//横向平铺
		bool bTiledY;			//纵向平铺
		bool bHSL;				//HSL色彩模式
		RECT rcPadding;			//外边距
		UINT uAlign;			//对齐方式
		int width;				//SVG的宽度
		int height;				//SVG的高度
		DWORD fillcolor;		//SVG的填充颜色
	} TDrawInfo;

	/////////////////////////////////////////////////////////////////////////////////////
	//
	enum emRenderEngine
	{
		DuiLib_Render_Gdi,  // 默认的，就是原来的模式。
		DuiLib_Render_GdiPlus,  // 统一使用Gdiplus绘制
		DuiLib_Render_Skia, // 使用 skia 图像库
        DuiLib_Render_D2D,  // 使用 d2d 绘制
	};

	//
	enum emUIOBJTYPE
	{
		OT_NULL=0,
		OT_HDC,
		OT_FONT,
		OT_PEN,
		OT_BRUSH,
		OT_PATH,
		OT_BITMAP,
		OT_IMAGE,
	};
	class CXmlNodeUI;
	class UILIB_API UIObject : public IObjRef
	{
	public:
		virtual const	emUIOBJTYPE ObjectType() const	= 0;	//{ return OT_NULL; }
		virtual void	DeleteObject()					= 0;
	};

	class UILIB_API UIFont : public TObjRefImpl<UIObject>
	{
	protected:
		UIFont()
		{
			id			= -1;
			iSize		= 16;
			bBold		= false;
			bUnderline	= false;
			bItalic		= false;
			bDefault	= false;
			bShared		= false;
		}
		virtual ~UIFont() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_FONT;  }

		virtual HFONT GetHFont(CPaintManagerUI *pManager) = 0;

		//构造默认字体
		BOOL CreateDefaultFont();

		//从参数构造字体
		BOOL CreateFont(CPaintManagerUI *pManager, int id, LPCTSTR sFontName, int iSize, 
			bool bBold, bool bUnderline, bool bItalic, bool bDefault, bool bShared);

		//从Xml节点构造字体, 如：<Font id="0" name=....... />
		BOOL CreateFont(CPaintManagerUI *pManager, CXmlNodeUI node);

		//重新构造字体, 当dpi改动时
		BOOL RebuildFont(CPaintManagerUI *pManager);

		//克隆字体
		virtual UIFont* Clone(CPaintManagerUI *pManager) = 0;

		//获取字体高度
		virtual int GetHeight(CPaintManagerUI *pManager) = 0;

		virtual int GetId()					{ return id;			}
		virtual CDuiString GetFontName()	{ return sFontName;		}
		virtual int GetSize()				{ return iSize;			}
		virtual bool IsBold()				{ return bBold;			}
		virtual bool IsUnderline()			{ return bUnderline;	}
		virtual bool IsItalic()				{ return bItalic;		}
		virtual bool IsDefault()			{ return bDefault;		}
		virtual bool IsShared()				{ return bShared;		}

	protected:
		virtual BOOL _buildFont(CPaintManagerUI *pManager=nullptr) = 0;
	protected:
		int id;
		CDuiString sFontName;
		int iSize;
		bool bBold;
		bool bUnderline;
		bool bItalic;
		bool bDefault;
		bool bShared;
	};

	class UILIB_API UIPen  : public TObjRefImpl<UIObject>
	{
	protected:
		UIPen();
		virtual ~UIPen() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_PEN;  }
		virtual HPEN GetHPen() const = 0;

		virtual BOOL CreatePen(int nStyle, int nWidth, DWORD dwColor) = 0;

		bool operator == (UIPen &obj) const;

		int GetStyle() const		{ return m_nStyle;	}
		int GetWidth() const		{ return m_nWidth;	}
		DWORD GetColor() const		{ return m_dwColor; }
	protected:
		int m_nStyle;
		int m_nWidth;
		DWORD m_dwColor;
	};

	class UILIB_API UIBitmap  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIBitmap() {}
	public:	
		const emUIOBJTYPE ObjectType() const override {  return OT_BITMAP;  }

		//hBitmap由内部释放
		virtual BOOL CreateFromHBitmap(HBITMAP hBitmap) = 0;

		virtual BOOL CreateARGB32Bitmap(HDC hDC, int width, int height) = 0;

		virtual BOOL CreateCompatibleBitmap(HDC hDC, int width, int height) = 0;

		virtual HBITMAP GetBitmap()		= 0;
		virtual BYTE* GetBits()			= 0;
		virtual int	GetWidth()			= 0;
		virtual int GetHeight()			= 0;

		virtual void Clear() = 0;

		virtual void ClearAlpha(const RECT &rc, int alpha = 0) = 0;

		virtual BOOL SaveFile(LPCTSTR pstrFileName) = 0;
	};

	class UILIB_API UIImage  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIImage();

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_IMAGE;  }

		virtual BOOL CreateImage(HBITMAP hBitmap, bool bAlpha) = 0;

		virtual BOOL LoadImage(const TDrawInfo *pDrawInfo, CPaintManagerUI* pManager=nullptr, HINSTANCE instance = nullptr) = 0;
		virtual BOOL LoadImage(STRINGorID bitmap, LPCTSTR type = nullptr, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=nullptr, HINSTANCE instance = nullptr) = 0;
		virtual BOOL LoadImage(LPCTSTR pStrImage, LPCTSTR type = nullptr, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=nullptr, HINSTANCE instance = nullptr) = 0;
		virtual BOOL LoadImage(UINT nID, LPCTSTR type = nullptr, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=nullptr, HINSTANCE instance = nullptr) = 0;
		
		//从内存中载入图像，支持图像格式：bmp, jpg, png, svg
		virtual BOOL LoadImageFromMemory(const LPBYTE pData, DWORD dwSize, DWORD mask=0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=nullptr) = 0;

		//图像HSL转换
		virtual void AdjustHslImage(bool bUseHSL, short H, short S, short L) = 0;

		//从文件中载入gif图像, arrImageInfo返回每帧图像， 使用完记得释放arrImageInfo
		virtual bool LoadGifImageFromFile(LPCTSTR fileName, CStdPtrArray &arrImageInfo) = 0;

		//从内存中载入gif图像, arrImageInfo返回每帧图像， 使用完记得释放arrImageInfo
		virtual bool LoadGifImageFromMemory(const LPBYTE pData, DWORD dwSize, CStdPtrArray &arrImageInfo) = 0;

		int GetWidth() const { return nWidth; }
		int GetHeight() const { return nHeight; }
        HDC GetDC() const throw();
        void ReleaseDC() const throw();
        bool Draw(
            HDC hDestDC,
            int xDest,
            int yDest,
            int nDestWidth,
            int nDestHeight) const throw();
        bool Draw(HDC hDestDC,
            int xDest,
            int yDest,
            int nDestWidth,
            int nDestHeight,
            int xSrc,
            int ySrc,
            int nSrcWidth,
            int nSrcHeight) const throw();
        bool AlphaBlend(
		    HDC hDestDC,
		    const RECT& rectDest,
		    const RECT& rectSrc,
		    BYTE bSrcAlpha = 0xff,
		    BYTE bBlendOp = AC_SRC_OVER) const throw();
        bool AlphaBlend(
            HDC hDestDC,
            int xDest,
            int yDest,
            int nDestWidth,
            int nDestHeight,
            int xSrc,
            int ySrc,
            int nSrcWidth,
            int nSrcHeight,
            BYTE bSrcAlpha = 0xff,
            BYTE bBlendOp = AC_SRC_OVER) const throw();
        bool BitBlt(
            HDC hDestDC,
            int xDest,
            int yDest,
            int nDestWidth,
            int nDestHeight,
            int xSrc,
            int ySrc,
            DWORD dwROP) const throw();
        void Destroy() throw();
        HBITMAP Detach() throw();
	public:
		UIBitmap *bitmap;		//图像数据
		LPBYTE pBits;			//转换HSL时会用到的
		LPBYTE pSrcBits;		//转换HSL时会用到的，保存转换之前的pBits
		int nWidth;				//宽度
		int nHeight;			//高度
		bool bAlpha;			//透明
		bool bUseHSL;			//HSL模式
		CDuiString sResType;	//资源文件中定义的类型，如："ZIPRES", "PNG"
		DWORD dwMask;			//透明色
		int delay;				//gif动画每帧延时
    private:
        mutable int m_nDCRefCount;
        mutable HDC m_hDC;
        mutable HBITMAP m_hOldBitmap;
    private:
        class CDCCache
        {
        public:
            CDCCache() throw();
            ~CDCCache() throw();
            HDC GetDC() throw();
            void ReleaseDC(_In_ HDC) throw();
        private:
            HDC m_ahDCs[4];
        };

        static CDCCache* GetCDCCacheInstance() {
            static CDCCache cache;
            return &cache;
        };
    };

	class UILIB_API UIBrush  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIBrush() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_BRUSH;  }
		virtual HBRUSH GetHBrush() const = 0;

		//hBrush由内部释放
		virtual BOOL CreateFromHBrush(HBRUSH hBrush) = 0;

		//用指定的实线初始化画刷
		virtual BOOL CreateSolidBrush(DWORD clr) = 0;

		//从位图构造画刷
		virtual BOOL CreateBitmapBrush(UIBitmap *bitmap) = 0;
	};

	class UILIB_API UIPath  : public TObjRefImpl<UIObject>
	{
	protected:
		virtual ~UIPath() {}

	public:
		const emUIOBJTYPE ObjectType() const override {  return OT_PATH;  }

		//开始定义新的路径 
		virtual BOOL Beginpath()	= 0;

		//结束定义路径
		virtual BOOL EndPath()		= 0;

		//结束定义路径 并且 清理
		virtual BOOL AbortPath()	= 0;

		//由多个线段组成的多边形
		virtual BOOL AddLine(int x1, int y1, int x2, int y2) = 0;

		//由多个点组成的多边形
		virtual BOOL AddLines(CDuiPoint *points, int count) = 0;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIRender : public IObjRef
	{
	protected:
		virtual ~UIRender() {}

	public:
		virtual void Init(CPaintManagerUI *pManager, HDC hDC = nullptr) = 0;
		virtual void AttachDC(CPaintManagerUI *pManager, HDC hDC) = 0;

		virtual CPaintManagerUI *GetManager()	= 0;
		virtual HDC GetDC() = 0;
		virtual UIBitmap *GetBitmap() = 0;

		virtual bool Resize(int width, int height) = 0;
		virtual bool Resize(const RECT &rc) = 0;

		//擦除当前画布。
		virtual void Clear() = 0;

		virtual void SaveDC() = 0;
		virtual void RestoreDC() = 0;

		virtual CStdRefPtr<UIObject> SelectObject(UIObject *pObject) = 0;
		virtual void RestoreObject(UIObject *pObject = nullptr) = 0;
		virtual void RestoreDefaultObject() = 0;

		//BitBlt (bit block transfer)
		virtual BOOL BitBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop = SRCCOPY ) = 0;
		
		//stretch BitBlt
		virtual BOOL StretchBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop = SRCCOPY) = 0;

		//绘制位图
		virtual void DrawBitmap(HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade = 255, bool hole = false, bool xtiled = false, bool ytiled = false) = 0;

		virtual void DrawBitmapHDC(HDC dc, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade = 255, bool hole = false, bool xtiled = false, bool ytiled = false) = 0;

		//画颜色(填充颜色)
		virtual void DrawColor(const RECT& rc, const SIZE &round, DWORD color) = 0;

		//画渐变色
		virtual void DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) = 0;

		//画线  MoveTo(x1, y1)，LineTo(x2, y2)
		virtual void DrawLine(int x1, int y1, int x2, int y2, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) = 0;

		//画边框
		virtual void DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) = 0;

		//画圆角边框
		virtual void DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle = PS_SOLID) = 0;

		//画圆或椭圆
		virtual void DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID) = 0;

		//填充圆或椭圆
		virtual void FillEllipse(const RECT& rc, DWORD dwColor) = 0;

		//绘制文本
		virtual void DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle) = 0;

		//建立路径
		virtual UIPath* CreatePath() = 0;	

		//绘制路径
		virtual BOOL DrawPath(const UIPath* path, int nSize, DWORD dwColor) = 0;

		//填充路径
		virtual BOOL FillPath(const UIPath* path, const DWORD dwColor) = 0;

		//绘制文字的占用的空间大小
		virtual SIZE GetTextSize(LPCTSTR pstrText, int iFont, UINT uStyle) = 0;

		//////////////////////////////////////////////////////////////////////////

		//画背景色
		void DrawBackColor(const RECT& rc, const SIZE &round, DWORD dwBackColor, DWORD dwBackColor2=0, DWORD dwBackColor3=0, bool bVertical=true);

		//画线  MoveTo(rc.left, rc.top)，LineTo(rc.right, rc.bottom)
		void DrawLine(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID);

		//画边框
		void DrawBorder(const RECT &rcItem, int nBorderSize, SIZE szBorderRound, RECT rcBorderSize, DWORD dwColor, int nBorderStyle);

		//绘制文本
		void DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle);

		//绘制带背景颜色的文本
		void DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText,DWORD dwTextColor, int iFont, UINT uStyle, DWORD dwTextBKColor);

		//根据TDrawInfo绘制图像
		bool DrawImageInfo(const RECT& rcItem, const RECT& rcPaint, const TDrawInfo* pDrawInfo, HINSTANCE instance = nullptr);

		//根据字符串属性绘制图像
		bool DrawImageString(const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify = nullptr, HINSTANCE instance = nullptr);

	protected:
		CPaintManagerUI *m_pManager;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIClip
	{
	public:
		UIClip();
		virtual ~UIClip();
	public:
		void GenerateClip(UIRender *pRender, RECT rc);
		void GenerateRoundClip(UIRender *pRender, RECT rc, RECT rcItem, int roundX, int roundY);
		void UseOldClipBegin(UIRender *pRender);
		void UseOldClipEnd(UIRender *pRender);

	protected:
		RECT m_rcItem;
		SIZE m_szRound;
		HDC m_hDC;
		HRGN m_hRgn;
		HRGN m_hOldRgn;
	};

	class UILIB_API UIRenderFactory : public IObjRef
	{
	protected:
		virtual ~UIRenderFactory() {}
	public:
		virtual UIRender *CreateRenderTarget()		= 0;

		virtual UIFont *CreateFont()				= 0;
		virtual UIPen *CreatePen()					= 0;
		virtual UIBitmap *CreateBitmap()			= 0;
		virtual UIBrush *CreateBrush()				= 0;
		virtual UIImage *CreateImage()				= 0;
	};

} // namespace DuiLib

#endif // __UIRENDER_H__

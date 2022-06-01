#ifndef __UIDATETIME_H__
#define __UIDATETIME_H__

#pragma once

namespace DuiLib
{
	class CDateTimeWnd;

	/// 时间选择控件
	class UILIB_API CDateTimeUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CDateTimeUI)
		friend class CDateTimeWnd;
	public:
		CDateTimeUI();
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		SYSTEMTIME& GetTime();
		void SetTime(SYSTEMTIME* pst);

		void SetFormatStyle(UINT uStyle);
		UINT GetFormatStyle();

		virtual void SetText(LPCTSTR pstrText);
		virtual CDuiString GetText() const;

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void DoEvent(TEventUI& event);

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		SYSTEMTIME m_sysTime;
		int        m_uFormatStyle; //0=date, 1=time, 2=datetime  add by liqs99

		CDateTimeWnd* m_pWindow;
	};
}
#endif // __UIDATETIME_H__
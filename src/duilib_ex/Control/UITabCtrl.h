#ifndef __UITABCTRL_H__
#define __UITABCTRL_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTabCtrlUI : public COptionUI
	{
		DECLARE_DUICONTROL(CTabCtrlUI)
	public:
#ifdef __MINGW32__
        using __super = COptionUI;
#endif
		CTabCtrlUI();
		~CTabCtrlUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual bool Activate() override;
		virtual void DoEvent(TEventUI& event) override;

		virtual bool IsSelected() const override;	
		virtual bool IsSelectedState() const override;
		virtual void BindTriggerTabSel(int _SetSelectIndex = -1) override;	

		void SetActiveMouseOn(bool bActive); //Êó±êÐüÍ£Ê±ÇÐ»»Ò³Ãæ
		bool IsActiveMouseOn();

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	protected:
		bool		m_bActiveMouseOn;
	};

} // namespace DuiLib

#endif // __UITABCTRL_H__

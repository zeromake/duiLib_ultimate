#pragma once

namespace DuiLib
{

class UILIB_API CPictureBoxUI : public CPictureUI
{
	DECLARE_DUICONTROL(CPictureBoxUI)
public:
#ifdef __MINGW32__
    using __super = CPictureUI;
#endif
	CPictureBoxUI(void);
	virtual ~CPictureBoxUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID	GetInterface(LPCTSTR pstrName) override;

	virtual void DoInit() override;
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	virtual bool LoadHBitmap(HBITMAP hBitmap);
	virtual bool LoadImageFromMemory(LPBYTE pData, DWORD dwSize);
	virtual bool LoadFile(LPCTSTR pstrImage);
};

}

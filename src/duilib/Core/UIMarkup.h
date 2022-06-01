#ifndef __UIMARKUP_H__
#define __UIMARKUP_H__

#pragma once

namespace DuiLib {

	enum
	{
        XMLFILE_ENCODING_AUTO = -1,
		XMLFILE_ENCODING_UTF8 = 0,
		XMLFILE_ENCODING_UNICODE = 1,
		XMLFILE_ENCODING_ASNI = 2
	};

    class IMarkupNode {
        public:
            virtual bool IsValid() const = 0;
            virtual IMarkupNode* GetParent() = 0;
            virtual IMarkupNode* GetSibling() = 0;
            virtual IMarkupNode* GetChild() = 0;
            virtual IMarkupNode* GetChild(LPCTSTR pstrName) = 0;

            virtual bool HasSiblings() const = 0;
            virtual bool HasChildren() const = 0;
            virtual LPCTSTR GetName() const = 0;
            virtual LPCTSTR GetValue() const = 0;

            virtual bool HasAttributes() = 0;
            virtual bool HasAttribute(LPCTSTR pstrName) = 0;
            virtual int GetAttributeCount() = 0;
            virtual LPCTSTR GetAttributeName(int iIndex) = 0;
            virtual LPCTSTR GetAttributeValue(int iIndex) = 0;
            virtual LPCTSTR GetAttributeValue(LPCTSTR pstrName) = 0;
            virtual bool GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax) = 0;
            virtual bool GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax) = 0;
    };

    class IMarkup {
        public:
            virtual bool Load(LPCTSTR pstrXML) = 0;
            virtual bool LoadFromMem(BYTE* pByte, DWORD dwSize, int encoding = XMLFILE_ENCODING_UTF8) = 0;
            virtual bool LoadFromFile(LPCTSTR pstrFilename, int encoding = XMLFILE_ENCODING_AUTO) = 0;
            virtual void Release() = 0;
            virtual bool IsValid() const = 0;

            virtual void SetPreserveWhitespace(bool bPreserve = true) = 0;
            virtual void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const = 0;
            virtual void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const = 0;
            virtual IMarkupNode* GetRoot() = 0;
    };

	class CMarkup;
	class CMarkupNode;

	class UILIB_API CMarkup: public IMarkup
	{
		friend class CMarkupNode;
	public:
		CMarkup(LPCTSTR pstrXML = NULL);
		~CMarkup();

		bool Load(LPCTSTR pstrXML) override;
		bool LoadFromMem(BYTE* pByte, DWORD dwSize, int encoding = XMLFILE_ENCODING_UTF8) override;
		bool LoadFromFile(LPCTSTR pstrFilename, int encoding = XMLFILE_ENCODING_AUTO) override;
		void Release() override;
		bool IsValid() const override;

		void SetPreserveWhitespace(bool bPreserve = true) override;
		void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const override;
		void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const override;
		IMarkupNode* GetRoot() override;
		bool _Failed(LPCTSTR pstrError, LPCTSTR pstrLocation = NULL);

	private:
		typedef struct tagXMLELEMENT
		{
			ULONG iStart;
			ULONG iChild;
			ULONG iNext;
			ULONG iParent;
			ULONG iData;
		} XMLELEMENT;

		LPTSTR m_pstrXML;
		XMLELEMENT* m_pElements;
		ULONG m_nElements;
		ULONG m_nReservedElements;
		TCHAR m_szErrorMsg[100];
		TCHAR m_szErrorXML[50];
		bool m_bPreserveWhitespace;

	private:
		bool _Parse();
		bool _Parse(LPTSTR& pstrText, ULONG iParent);
		XMLELEMENT* _ReserveElement();
		inline void _SkipWhitespace(LPTSTR& pstr) const;
		inline void _SkipWhitespace(LPCTSTR& pstr) const;
		inline void _SkipIdentifier(LPTSTR& pstr) const;
		inline void _SkipIdentifier(LPCTSTR& pstr) const;
		bool _ParseData(LPTSTR& pstrText, LPTSTR& pstrData, char cEnd);
		void _ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest);
		bool _ParseAttributes(LPTSTR& pstrText);
	};


	class UILIB_API CMarkupNode: public IMarkupNode
	{
		friend class CMarkup;
	public:
		CMarkupNode();
		CMarkupNode(CMarkup* pOwner, int iPos);

	public:
		bool IsValid() const override;

		IMarkupNode* GetParent() override;
		IMarkupNode* GetSibling() override;
		IMarkupNode* GetChild() override;
		IMarkupNode* GetChild(LPCTSTR pstrName) override;

		bool HasSiblings() const override;
		bool HasChildren() const override;
		LPCTSTR GetName() const override;
		LPCTSTR GetValue() const override;

		bool HasAttributes() override;
		bool HasAttribute(LPCTSTR pstrName) override;
		int GetAttributeCount() override;
		LPCTSTR GetAttributeName(int iIndex) override;
		LPCTSTR GetAttributeValue(int iIndex) override;
		LPCTSTR GetAttributeValue(LPCTSTR pstrName) override;
		bool GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax) override;
		bool GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax) override;

	private:
		void _MapAttributes();

		enum { MAX_XML_ATTRIBUTES = 64 };

		typedef struct
		{
			ULONG iName;
			ULONG iValue;
		} XMLATTRIBUTE;

		int m_iPos;
		int m_nAttributes;
		XMLATTRIBUTE m_aAttributes[MAX_XML_ATTRIBUTES];
		CMarkup* m_pOwner;
	};

} // namespace DuiLib

#endif // __UIMARKUP_H__

#include "StdAfx.h"

namespace DuiLib {
///////////////////////////////////////////////////////////////////////////////////////
//
//
//
inline ui_pugi::xml_document *impxmldoc(PVOID pdoc) { return (ui_pugi::xml_document *)pdoc; }

CXmlDocumentUI::CXmlDocumentUI()
{
	_xml_document = new ui_pugi::xml_document;
}

CXmlDocumentUI::~CXmlDocumentUI()
{
	delete (ui_pugi::xml_document *)_xml_document;
}

bool CXmlDocumentUI::load_string(LPCTSTR pstrXML)
{
	ui_pugi::xml_parse_result ret = impxmldoc(_xml_document)->load_string((ui_pugi::char_t*)pstrXML, ui_pugi::parse_full);
	if(ret.status != ui_pugi::status_ok)
	{
		_root = impxmldoc(_xml_document)->root().internal_object();
		UISTRING_CONVERSION;
		return _Failed(UIUTF82T(ret.description()));
	}
	_root = impxmldoc(_xml_document)->root().internal_object();
	return true;
}

bool CXmlDocumentUI::load_buffer(const void* contents, size_t size)
{
    if (contents == NULL) return false;
	UISTRING_CONVERSION;
#ifdef _UNICODE
	ui_pugi::xml_parse_result ret = impxmldoc(_xml_document)->load_buffer(contents, size, ui_pugi::parse_full);
#else
	// LPCTSTR strContent = UIUTF82T(contents);
    // _tprintf("utf-8: %s\n", (TCHAR*)contents);
	ui_pugi::xml_parse_result ret = impxmldoc(_xml_document)->load_buffer(contents, size, ui_pugi::parse_full);
#endif

	if(ret.status != ui_pugi::status_ok)
	{
		_root = impxmldoc(_xml_document)->root().internal_object();
		return _Failed(UIUTF82T(ret.description()));
	}
	_root = impxmldoc(_xml_document)->root().internal_object();
	return true;
}

bool CXmlDocumentUI::load_file(LPCTSTR pstrFilename)
{
	m_sFileName = pstrFilename;
	CUIFile file;
	if (!file.LoadFile(pstrFilename)) {
        return false;
    }
	return load_buffer(file.GetData(), file.GetSize());
}


bool CXmlDocumentUI::save_file(LPCTSTR path)
{
#ifdef UNICODE
	return impxmldoc(_xml_document)->save_file((wchar_t*)path, (ui_pugi::char_t*)_T("\t"), ui_pugi::format_default,  ui_pugi::encoding_utf8);
#else
	return impxmldoc(_xml_document)->save_file((char*)path, (ui_pugi::char_t *)_T("\t"), ui_pugi::format_default,  ui_pugi::encoding_utf8);
#endif
}

bool CXmlDocumentUI::save_to_default_file()
{
#ifdef UNICODE
	return impxmldoc(_xml_document)->save_file((wchar_t*)m_sFileName.GetData(), (ui_pugi::char_t*)_T("\t"), ui_pugi::format_default,  ui_pugi::encoding_utf8);
#else
	return impxmldoc(_xml_document)->save_file((char*)m_sFileName.GetData(), (ui_pugi::char_t *)_T("\t"), ui_pugi::format_default,  ui_pugi::encoding_utf8);
#endif
}

CDuiString CXmlDocumentUI::GetError()
{
	return m_sErrorMsg;
}

bool CXmlDocumentUI::_Failed(LPCTSTR pstrError)
{
	m_sErrorMsg = pstrError;
	return false; // Always return 'false'
}

} // namespace DuiLib

#include "StdAfx.h"
#include "resource.h"
#include "MainWnd.h"

void InitResource()
{	
	// 资源类型
#ifdef _DEBUG
	//CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);

	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// 加载资源
	switch(CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
		{
			strResourcePath += _T("Skin\\TroyBrowser\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_RESOURCE:
		{
			strResourcePath += _T("Skin\\TroyBrowser\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
			break;
		}
	case UILIB_ZIP:
		{
			strResourcePath += _T("Skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CPaintManagerUI::SetResourceZip(_T("TroyBrowser.zip"), true);
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
			strResourcePath += _T("Skin\\TroyBrowser\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
			if( hResource != NULL ) {
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
				if( hGlobal != NULL ) {
					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
					if( dwSize > 0 ) {
						CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
						// 加载资源管理器
						CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
					}
				}
				::FreeResource(hGlobal);
			}
		}
		break;
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	// 初始化资源
	InitResource();

	::SetCurrentDirectory(CPaintManagerUI::GetInstancePath());
#ifndef _DEBUG
	CPaintManagerUI::LoadPlugin(_T("TroyControls.dll"));
#else
	CPaintManagerUI::LoadPlugin(_T("TroyControls_d.dll"));
#endif
	CMainWnd* pFrame = new CMainWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("TroyBrowser"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 990,690);
	pFrame->CenterWindow();
	pFrame->ShowModal();

	CPaintManagerUI::MessageLoop();
	OleUninitialize();
	::CoUninitialize();
	return 0;
}
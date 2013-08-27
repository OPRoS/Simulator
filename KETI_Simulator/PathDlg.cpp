// PathDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KETI_Simulator.h"
#include "PathDlg.h"
#include "KETI_SimulatorDlg.h"


// CPathDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathDlg, CDialog)

CPathDlg::CPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPathDlg::IDD, pParent)
	, EnginePath(_T(""))
	, RepositoryPath(_T(""))
	, ModelPath(_T(""))
	, EngineName(_T(""))
	, devicePortNumber(0)
	, BatchFile(_T(""))
{

}

CPathDlg::~CPathDlg()
{
}

void CPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, EnginePath);
	DDX_Text(pDX, IDC_EDIT2, RepositoryPath);
	DDX_Text(pDX, IDC_EDIT3, ModelPath);
	DDX_Text(pDX, IDC_EDIT4, EngineName);
	DDX_Text(pDX, IDC_EDIT5, devicePortNumber);
	DDX_Text(pDX, IDC_EDIT6, BatchFile);
}


BEGIN_MESSAGE_MAP(CPathDlg, CDialog)
	ON_BN_CLICKED(IDC_ENGINEPATHBTN, &CPathDlg::OnBnClickedEnginepathbtn)
	ON_BN_CLICKED(IDC_REPOSITORYBTN, &CPathDlg::OnBnClickedRepositorybtn)
	ON_BN_CLICKED(IDC_MODELBTN, &CPathDlg::OnBnClickedModelbtn)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPathDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDOK, &CPathDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BATCHFILE, &CPathDlg::OnBnClickedBatchfile)
END_MESSAGE_MAP()


// CPathDlg 메시지 처리기입니다.
HRESULT CPathDlg::SHPathToPidl(LPCSTR szPath, LPITEMIDLIST* ppidl)
{
	LPSHELLFOLDER pShellFolder = NULL;
	OLECHAR wszPath[1024] = {0};
	ULONG nCharsParsed = 0;
	HRESULT hr = SHGetDesktopFolder( &pShellFolder );
	if( FAILED(hr) ) return FALSE;
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szPath, -1, wszPath, MAX_PATH );
	hr = pShellFolder->ParseDisplayName( NULL, NULL, wszPath, &nCharsParsed, ppidl, NULL );
	pShellFolder->Release();

	return E_NOTIMPL;
}

void CPathDlg::OnBnClickedEnginepathbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("exe files(*.exe)|*.exe|"));
	if (dlg.DoModal() == IDOK)
	{
		EnginePath = dlg.GetPathName().Left((dlg.GetPathName().ReverseFind(0x5C)));
		EngineName = dlg.GetFileName();
		UpdateData(FALSE);
	}
}

void CPathDlg::OnBnClickedRepositorybtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ITEMIDLIST *pildBrowse;
	wchar_t pszPathname[1024];

	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bInfo;
	ZeroMemory( &bInfo, sizeof(BROWSEINFO) );
	SHPathToPidl( CSIDL_DESKTOP, &pidl );       // 루트 디렉토리 설정 부분.
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.pidlRoot = pidl;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle = _T("Please Select Directory....");
	bInfo.lpfn = NULL;
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_VALIDATE ;
	pildBrowse = ::SHBrowseForFolder(&bInfo);

	if( pildBrowse != NULL )
	{
		SHGetPathFromIDList(pildBrowse, pszPathname);
		RepositoryPath = pszPathname;
		UpdateData(FALSE);                               
	}
}

void CPathDlg::OnBnClickedModelbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ITEMIDLIST *pildBrowse;
	wchar_t pszPathname[1024];

	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bInfo;
	ZeroMemory( &bInfo, sizeof(BROWSEINFO) );
	SHPathToPidl( CSIDL_DESKTOP, &pidl );       // 루트 디렉토리 설정 부분.
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.pidlRoot = pidl;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle = _T("Please Select Directory....");
	bInfo.lpfn = NULL;
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_VALIDATE ;
	pildBrowse = ::SHBrowseForFolder(&bInfo);

	if( pildBrowse != NULL )
	{
		SHGetPathFromIDList(pildBrowse, pszPathname);
		ModelPath = pszPathname;
		UpdateData(FALSE);                               
	}
}

BOOL CPathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	BSTR str;

#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif

	g_pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
	g_pXMLDoc->async = VARIANT_FALSE;
    g_pXMLDoc->validateOnParse = VARIANT_TRUE;
    g_pXMLDoc->resolveExternals = VARIANT_TRUE;
	g_pXMLDoc->load(_T("syspath.xml"));

	MSXML2::IXMLDOMParseErrorPtr pError = g_pXMLDoc->parseError;
	if (pError->errorCode != S_OK)
	{
		AfxMessageBox(_T("syspath.xml 파일이 존재하지 않거나 손상되었습니다"));
	}
	else
	{
		//AfxMessageBox(_T("xml 로드가 성공적으로 이루어졌습니다."));
		MSXML2::IXMLDOMNodePtr EnginePathNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/EnginePath"));
		str =_bstr_t("");
		str = EnginePathNodePtr->Gettext();
		USES_CONVERSION;
		EnginePath = W2A(str);

		MSXML2::IXMLDOMNodePtr EngineNamePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/EngineName"));
		str = bstr_t("");
		str = EngineNamePtr->Gettext();
		EngineName = W2A(str);

		MSXML2::IXMLDOMNodePtr EngineBatchPtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("PATH/BatchFile"));
		str = bstr_t("");
		str = EngineBatchPtr->Gettext();
		BatchFile = W2A(str);

		MSXML2::IXMLDOMNodePtr RepositoryPathNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/RepositoryPath"));
		str =_bstr_t("");
		str = RepositoryPathNodePtr->Gettext();
		RepositoryPath = W2A(str);

		MSXML2::IXMLDOMNodePtr ModelPathNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/ModelPath"));
		str =_bstr_t("");
		str = ModelPathNodePtr->Gettext();
		ModelPath = W2A(str);

		MSXML2::IXMLDOMNodePtr PortNodePrt = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/Port"));
		str =_bstr_t("");
		str = PortNodePrt->Gettext();
		devicePortNumber = _ttoi(str);
		UpdateData(FALSE);		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPathDlg::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MSXML2::IXMLDOMNodePtr EnginePathNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/EnginePath"));
	MSXML2::IXMLDOMNodePtr EngineNameNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/EngineName"));
	MSXML2::IXMLDOMNodePtr BatchFileNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/BatchFile"));
	MSXML2::IXMLDOMNodePtr RepositoryPathNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/RepositoryPath"));
	MSXML2::IXMLDOMNodePtr ModelPathNodePtr = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/ModelPath"));
	MSXML2::IXMLDOMNodePtr PortNodePrt = 
			g_pXMLDoc->selectSingleNode(_bstr_t("/PATH/Port"));

	UpdateData(TRUE);
	CString b_str;
	b_str.Format(_T("%d"),devicePortNumber);

	EnginePathNodePtr->Puttext(EnginePath.AllocSysString());
	EngineNameNodePtr->Puttext(EngineName.AllocSysString());
	BatchFileNodePtr->Puttext(BatchFile.AllocSysString());
	RepositoryPathNodePtr->Puttext(RepositoryPath.AllocSysString());
	ModelPathNodePtr->Puttext(ModelPath.AllocSysString());
	PortNodePrt->Puttext(b_str.AllocSysString());


	((CKETI_SimulatorDlg*)GetParent())->EnginePath = EnginePath;
	((CKETI_SimulatorDlg*)GetParent())->RepositoryPath = RepositoryPath;
	((CKETI_SimulatorDlg*)GetParent())->EngineBatchFile = BatchFile;
	((CKETI_SimulatorDlg*)GetParent())->ModelPath = ModelPath;
	((CKETI_SimulatorDlg*)GetParent())->EngineName = EngineName;
	((CKETI_SimulatorDlg*)GetParent())->PortNumber = devicePortNumber;


	g_pXMLDoc->save(_T("syspath.xml"));
	AfxMessageBox(_T("저장되었습니다."));

}

void CPathDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	OnOK();
}

void CPathDlg::OnBnClickedBatchfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("bat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("bat files(*.bat)|*.bat|"));
	if (dlg.DoModal() == IDOK)
	{
		//EnginePath = dlg.GetPathName().Left((dlg.GetPathName().ReverseFind(0x5C)));
		BatchFile = dlg.GetFileName();
		UpdateData(FALSE);
	}
}

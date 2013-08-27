#pragma once

#ifdef _DEBUG
#import <msxml4.dll>
using namespace	MSXML2;
#else
#endif

#import <msxml4.dll>
using namespace	MSXML2;

// CPathDlg ��ȭ �����Դϴ�.

class CPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CPathDlg)

public:
	CPathDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEnginepathbtn();
	HRESULT SHPathToPidl(LPCSTR szPath, LPITEMIDLIST* ppidl);
	CString EnginePath;
	CString RepositoryPath;
	CString ModelPath;
	//CString EngineName;
	afx_msg void OnBnClickedRepositorybtn();
	afx_msg void OnBnClickedModelbtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedOk();
	MSXML2::IXMLDOMDocument2Ptr g_pXMLDoc;
	CString EngineName;
	int devicePortNumber;
	CString BatchFile;
	afx_msg void OnBnClickedBatchfile();
};

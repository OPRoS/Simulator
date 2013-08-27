#include "StdAfx.h"
#include "OpenModelDlg.h"
#include "KETI_SimulatorDlg.h"


COpenModelDlg::~COpenModelDlg(void)
{
}


COpenModelDlg::COpenModelDlg(CWnd* pParent /*=NULL*/)
: CDialog(COpenModelDlg::IDD, pParent) ,m_modelname(_T("kobot")), m_filename(_T("")), m_posX(0), m_posY(0), m_posZ(0.2), m_rotX(0), m_rotY(0), m_rotZ(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_modelname);
	DDX_Text(pDX, IDC_FILE_NAME, m_filename);
	DDX_Text(pDX, IDC_POS_X, m_posX);
	DDX_Text(pDX, IDC_POS_Y, m_posY);
	DDX_Text(pDX, IDC_POS_Z, m_posZ);
	DDX_Text(pDX, IDC_ROT_X, m_rotX);
	DDX_Text(pDX, IDC_ROT_Y, m_rotY);
	DDX_Text(pDX, IDC_ROT_Z, m_rotZ);
}

BEGIN_MESSAGE_MAP(COpenModelDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_FILE_DLG, &COpenModelDlg::OnBnClickedOpenFileDlg)
	ON_BN_CLICKED(IDOK, &COpenModelDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKETI_SimulatorDlg message handlers

BOOL COpenModelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenModelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpenModelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpenModelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HTransform COpenModelDlg::GetHTransform() const
{
	HTransform T(Rotation(), Displacement(m_posX, m_posY, m_posZ));
	T.R.PreXRotate(m_rotX*DEGREE);
	T.R.PreYRotate(m_rotY*DEGREE);
	T.R.PreZRotate(m_rotZ*DEGREE);

	return T;
}


void COpenModelDlg::OnBnClickedOpenFileDlg()
{
	// TODO: Add your control notification handler code here
	
// 	TCHAR szFilter[] = _T("aml files(*.aml|*.aml")
// 		_T()

	UpdateData(TRUE);
	CFileDialog dlg(TRUE, _T("aml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("aml files(*.aml)|*.aml|eml files(*.eml)|*.eml|"));

	dlg.GetOFN().lpstrInitialDir = ((CKETI_SimulatorDlg*)GetParent())->ModelPath;
	//dlg.GetOFN().lpstrInitialDir = _T("C:\\Program Files\\RoboticsLab");
	if (dlg.DoModal() == IDOK)
	{
		m_filename = dlg.GetPathName();
		UpdateData(FALSE);
	}

}

void COpenModelDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_modelname.GetLength() == 0 || m_filename.GetLength() == 0)
	{
		AfxMessageBox(TEXT("모델 이름과 파일 이름을 지정해 주어야 합니다"));
		return;
	}
	OnOK();
}

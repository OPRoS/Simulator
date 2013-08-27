// KETI_SimulatorDlg.cpp : implementation file
//
#include "stdafx.h"
#include <process.h>
#include "KETI_Simulator.h"
#include "OpenModelDlg.h"
#include "Simulator.h"
#include "RobotController.h"
#include "SimulationController.h"
#include "KETI_SimulatorDlg.h"
#include "SimulatorData.h"
#include <crtdbg.h>

#include "OPRoSEngineRedirection.h"
#include "RobotControlData.h"
#include "SimulatorDebug.h"
#include "JoystickDlg.h"
#include "OPRoSJoystickControl.h"
#include "ManipulatorDlg.h"
#include "OPRoSManipulatorDlg.h"
#include "PathDlg.h"
#include "DepthView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef _DEBUG
#undef THIS_FILE
static	char	THIS_FILE[] = __FILE__;
#endif

static int ItemIndex = 0;
rxSystem* sys[3];
rxDevice* device[256];

// CAboutDlg dialog used for App About

void SimulationLoop(void* param)
{
	CKETI_SimulatorDlg* dlg = (CKETI_SimulatorDlg*)param;

	//dlg->m_simulator->initialize();

	while(dlg->m_bRunSimulation)
	{
		if(!dlg->m_simulator->update())	{
			Sleep(10);
		}
		dlg->m_simulator->getTime();
	}
	ExitThread(0);
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//ON_MESSAGE(SEND_LOG_MESSGE, )
END_MESSAGE_MAP()


// CKETI_SimulatorDlg dialog




CKETI_SimulatorDlg::CKETI_SimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKETI_SimulatorDlg::IDD, pParent) , m_simulator(NULL), m_simulationController(NULL), m_robotController(NULL), m_bRunSimulation(true)
{
	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(601);*/
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_aml_loaded	=	false;
	_eml_loaded	=	false;
}

CKETI_SimulatorDlg::~CKETI_SimulatorDlg()
{	
	m_bRunSimulation =false;
	WaitForSingleObject(m_hThread, INFINITE);

	if (m_simulationController)
		delete m_simulationController;

	if (m_robotController)
		delete m_robotController;

	if (m_simulator)
		delete m_simulator;
	
	if (m_oprosEngineRedirection)
		delete m_oprosEngineRedirection;

	if (m_RobotControlData)
		delete	m_RobotControlData;

	//if (m_JoystickDlg)
	//	delete	m_JoystickDlg;

	if (m_OPRoSJoystick)
		delete	m_OPRoSJoystick;

	if (m_Manipulator)
		delete	m_Manipulator;

	if (m_OPRoSManipulator)
		delete	m_OPRoSManipulator;

	if (m_SimulatorDebug)
		delete	m_SimulatorDebug;

}

void CKETI_SimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_MODEL_LIST, m_modelListCtrl);
	DDX_Control(pDX, IDC_LOG_EDIT, m_logMsg);
	DDX_Control(pDX, IDC_START_SIM_BTN, _btnStartSimulation);
	DDX_Control(pDX, IDC_STOP_SIM_BTN, _btnStopSimulation);
	DDX_Control(pDX, IDC_APP_LIST, AppList);
	DDX_Control(pDX, IDC_ENGINE_STATE, EngineState);
	DDX_Control(pDX, IDC_APP_RUNLIST, AppRunList);
	DDX_Control(pDX, IDC_TREE2, m_deviceTree);
	//DDX_Control(pDX, IDC_TAB2, m_debugTab);
	DDX_Control(pDX, IDC_DEBUGTAB, m_debugTab);
}

BEGIN_MESSAGE_MAP(CKETI_SimulatorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CKETI_SimulatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPEN_MODEL_BTN, &CKETI_SimulatorDlg::OnBnClickedOpenModelBtn)
	ON_BN_CLICKED(IDC_START_SIM_BTN, &CKETI_SimulatorDlg::OnBnClickedStartSimBtn)
	ON_BN_CLICKED(IDC_STOP_SIM_BTN, &CKETI_SimulatorDlg::OnBnClickedStopSimBtn)
	ON_MESSAGE(SEND_LOG_MESSGE, &CKETI_SimulatorDlg::OnLogMessage)
	ON_BN_CLICKED(IDC_UNLOAD_MODEL_BTN, &CKETI_SimulatorDlg::OnBnClickedUnloadModelBtn)
	ON_BN_CLICKED(IDC_CLEAR_BTN, &CKETI_SimulatorDlg::OnBnClickedClearBtn)
	ON_BN_CLICKED(IDC_RESET_BTN, &CKETI_SimulatorDlg::OnBnClickedResetBtn)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ENGINE_START, &CKETI_SimulatorDlg::OnBnClickedEngineStart)
	ON_BN_CLICKED(IDC_ENGINE_EXIT, &CKETI_SimulatorDlg::OnBnClickedEngineExit)
	ON_NOTIFY(NM_CLICK, IDC_APP_LIST, &CKETI_SimulatorDlg::OnNMClickAppList)
	ON_NOTIFY(NM_DBLCLK, IDC_APP_LIST, &CKETI_SimulatorDlg::OnNMDblclkAppList)
	ON_LBN_DBLCLK(IDC_APP_RUNLIST, &CKETI_SimulatorDlg::OnLbnDblclkAppRunlist)
	ON_COMMAND(ID_WHEELROBOT_SIMULATORCONTROL, &CKETI_SimulatorDlg::OnWheelrobotSimulatorcontrol)
	ON_COMMAND(ID_WHEELROBOT_OPROSENGINECONTROL, &CKETI_SimulatorDlg::OnWheelrobotOprosenginecontrol)
	ON_COMMAND(ID_MANIPULATOR_SIMULATORCONTROL, &CKETI_SimulatorDlg::OnManipulatorSimulatorcontrol)
	ON_COMMAND(ID_MANIPULATOR_OPROSENGINECONTROL, &CKETI_SimulatorDlg::OnManipulatorOprosenginecontrol)
	ON_COMMAND(ID_FILE_PATHSETTING, &CKETI_SimulatorDlg::OnFilePathsetting)
	//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CKETI_SimulatorDlg::OnTcnSelchangeTab2)
	//ON_NOTIFY(TCN_SELCHANGING, IDC_TAB2, &CKETI_SimulatorDlg::OnTcnSelchangingTab2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DEBUGTAB, &CKETI_SimulatorDlg::OnTcnSelchangeDebugtab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DEBUGTAB, &CKETI_SimulatorDlg::OnTcnSelchangingDebugtab)
	ON_NOTIFY(NM_CLICK, IDC_TREE2, &CKETI_SimulatorDlg::OnNMClickTree2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CKETI_SimulatorDlg message handlers

BOOL CKETI_SimulatorDlg::OnInitDialog()
{
	int clear_var = 0;
	CDialog::OnInitDialog();
	BSTR str;
	memset(sysPath ,0,255);
	GetModuleFileName(AfxGetApp()->m_hInstance, sysPath, 255);
	
	for (int i = 254 ; i > 0 ; i--)
	{
		if (sysPath[i] == '\\' )
		{
			clear_var = i;
			break;
		}
	}

	for (int i = clear_var ; i < 255 ; i++)
	{
		sysPath[i] = 0;
	}

	int Len = wcslen(sysPath);
	char* pstr = (char*) malloc (sizeof(char)* Len+1);
	wcstombs(pstr,sysPath,Len+1);
	strcat(pstr,"\\syspath.xml");

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	kaiInitialize();
	
	m_simulator = new Simulator();

	m_simulator->initialize();


	m_robotController = new RobotController();
	m_robotController->registerSimulator(m_simulator);
	m_simulationController = new SimulationController();
	m_simulationController->registerSimulator(m_simulator);
	m_simulatorData = new SimulatorData();
	m_simulatorData->registerSimulator(m_simulator);
	m_oprosEngineRedirection = new OPRoSEngineRedirection();
	m_oprosEngineRedirection->registerSimulator(m_simulator);
	m_RobotControlData = new RobotControlData();
	m_RobotControlData->registerSimulator(m_simulator);
	m_JoystickDlg = new CJoystickDlg();
	m_JoystickDlg->registerSimulator(m_simulator);
	m_OPRoSJoystick = new COPRoSJoystickControl();
	m_OPRoSJoystick->registerSimulator(m_simulator);
	m_Manipulator = new CManipulatorDlg();
	m_Manipulator->registerSimulator(m_simulator);
	m_OPRoSManipulator = new COPRoSManipulatorDlg();
	m_OPRoSManipulator->registerSimulator(m_simulator);
	m_SimulatorDebug = new CSimulatorDebug();
	m_SimulatorDebug->registerSimulator(m_simulator);

	IXMLDOMDocument2Ptr pPathData;
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif

	pPathData.CreateInstance(__uuidof(DOMDocument40));
	//pPathData->load(pstr);
	pPathData->load(_T("syspath.xml"));
	MSXML2::IXMLDOMParseErrorPtr pError = pPathData->parseError;
	if (pError->errorCode != S_OK)
	{
		AfxMessageBox(_T("syspath.xml 파일이 존재하지 않거나 손상되었습니다"));
	}
	else
	{
		MSXML2::IXMLDOMNodePtr EnginePathPtr = 
			pPathData->selectSingleNode(_bstr_t("/PATH/EnginePath"));
		str =_bstr_t("");
		str = EnginePathPtr->Gettext();
		USES_CONVERSION;
		EnginePath = W2A(str);

		MSXML2::IXMLDOMNodePtr EngineNamePtr = 
			pPathData->selectSingleNode(_bstr_t("/PATH/EngineName"));
		str = bstr_t("");
		str = EngineNamePtr->Gettext();
		EngineName = W2A(str);

		MSXML2::IXMLDOMNodePtr EngineBatchPtr = 
			pPathData->selectSingleNode(_bstr_t("PATH/BatchFile"));
		str = bstr_t("");
		str = EngineBatchPtr->Gettext();
		EngineBatchFile = W2A(str);

		MSXML2::IXMLDOMNodePtr RepositoryPathPtr = 
			pPathData->selectSingleNode(_bstr_t("/PATH/RepositoryPath"));
		str =_bstr_t("");
		str = RepositoryPathPtr->Gettext();
		RepositoryPath = W2A(str);

		MSXML2::IXMLDOMNodePtr ModelPathPtr = 
			pPathData->selectSingleNode(_bstr_t("PATH/ModelPath"));
		str =_bstr_t("");
		str = ModelPathPtr->Gettext();
		ModelPath = W2A(str);

		MSXML2::IXMLDOMNodePtr PortNodePrt = 
			pPathData->selectSingleNode(_bstr_t("/PATH/Port"));
		str =_bstr_t("");
		str = PortNodePrt->Gettext();
		PortNumber = _ttoi(str);
	}

	m_hThread = (HANDLE)_beginthread(SimulationLoop, 0, this);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_modelListCtrl.InsertColumn(0, TEXT("model name"), LVCFMT_LEFT, 100, 0);
	m_modelListCtrl.InsertColumn(1, TEXT("file name"), LVCFMT_LEFT, 100, 1);

	// TODO: Add extra initialization here

	m_modelListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	AppList.InsertColumn(0, TEXT("Application Profile List"), LVCFMT_LEFT, 130,0);
	AppList.InsertColumn(1, TEXT("Component List"),LVCFMT_LEFT, 150 , 1);
	AppList.SetExtendedStyle(LVS_EX_GRIDLINES);
		
	m_oprosEngineRedirection->LoadAppList();
	EngineState.SetWindowTextW(TEXT("OPRoS Engine Not Used"));
	InitializeCriticalSection(&_hCriticalSection);
	UpdateData(FALSE);

	//free(pstr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKETI_SimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKETI_SimulatorDlg::OnPaint()
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
HCURSOR CKETI_SimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKETI_SimulatorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}



/*	OnBnClickedUnloadModelBtn

	추가: 김영중
	선택된 모듈을 언로드
*/
void CKETI_SimulatorDlg::OnBnClickedUnloadModelBtn()
{
	// TODO: Add your control notification handler code here

	//	stop simulation
	OnBnClickedStopSimBtn();

	POSITION	pos	=	m_modelListCtrl.GetFirstSelectedItemPosition();
	//	리스트컨트롤에서 선택된 아이템 루핑

	if	(pos != NULL)	{

		do	{
			int	sel_index	=	m_modelListCtrl.GetNextSelectedItem(pos);
			int	model_id	=	m_modelListCtrl.GetItemData(sel_index);

			sItemInfo	&	info	=	m_itemInfoes[model_id];
			//	aml or eml
			if	(info.is_system)	{
				m_simulator->unloadModel(model_id);
				_aml_loaded	=	false;
			}
			else	{
				m_simulator->unloadEnv(model_id);
				_eml_loaded	=	false;
			}

			m_modelListCtrl.DeleteItem(sel_index);
			m_itemInfoes.erase(model_id);

			pos	=	m_modelListCtrl.GetFirstSelectedItemPosition();
		}
		while	(pos != NULL);

		//	모든 파일 언로드시 rPlayer 클리어
		if	(0 == m_simulator->system_size() && 0 == m_simulator->env_size())
			m_simulator->clearDisplay();
	}
}


void CKETI_SimulatorDlg::OnBnClickedOpenModelBtn()
{
	// TODO: Add your control notification handler code here

	COpenModelDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString modelname = dlg.GetModelName();
		CString filename = dlg.GetFileName();
	

		
		
		
		HTransform T0	=	dlg.GetHTransform();
		_transform_org	=	T0;
		rID	model_id	=	0;
		bool	is_system	=	true;

		if (m_simulator)
		{
			string_type	t_filename	=	filename.GetBuffer();
			string_type	t_modelname	=	modelname.GetBuffer();

			size_t pos = t_filename.find_last_of('.');
			string_type ext = t_filename.substr(pos+1, t_filename.size());
			
			//m_simulator->loadModel(filename.GetBuffer(), modelname.GetBuffer(), T0);

			if (ext == _T("aml"))	{
				/*
				if	(_aml_loaded)	{
					MessageBox(_T("한개의 AML만 로딩합니다"), _T("AML 로드"), MB_OK);
					return	;
				}
				*/

				is_system	=	true;
				model_id	=	m_simulator->loadSystem(t_filename, t_modelname, T0);
				_aml_loaded	=	true;
			}
			else	if	(ext == _T("eml"))	{
				if	(_eml_loaded)	{
					MessageBox(_T("한개의 EML만 로딩합니다"), _T("EML 로드"), MB_OK);
					return	;
				}

				is_system	=	false;
				model_id	=	m_simulator->loadEnvironment(t_filename, t_modelname, T0);
				_eml_loaded	=	true;
			}
		}

		int idx = m_modelListCtrl.InsertItem(ItemIndex++, modelname);
		m_modelListCtrl.SetItemText(idx, 1, filename);
		m_modelListCtrl.SetItemData(idx, model_id);

		sItemInfo info;
		info.filename = filename;
		info.modelname = modelname;
		info.id	=	idx;
		info.model_id = model_id;
		info.T0 = T0;
		info.is_system	=	is_system;
		m_itemInfoes[model_id]	=	info;
	}
}


void CKETI_SimulatorDlg::OnBnClickedStartSimBtn()
{
	// TODO: Add your control notification handler code here

	int robotCount = 0;
	int DeviceCount = 0;

	_btnStartSimulation.EnableWindow(FALSE);
	_btnStopSimulation.EnableWindow(TRUE);

	if (m_simulator)	{
		EnterCriticalSection(&_hCriticalSection);
		m_simulator->activate();
		LeaveCriticalSection(&_hCriticalSection);
	}

	map<int, sItemInfo>::const_iterator	itr	= m_itemInfoes.begin();
	
	for	(; itr != m_itemInfoes.end(); ++itr)	
	{
		if	(itr->second.is_system)	
		{
			m_SimulatorDebug->RobotID[robotCount]	= itr->second.model_id;
			m_SimulatorDebug->RobotName[robotCount]	= itr->second.modelname;
			sys[robotCount] = m_simulator->findSystem(itr->second.model_id);
			DeviceList& dList = sys[robotCount]->getDevices();
			robotCount++;
			for(DeviceList::iterator iter = dList.begin() ; iter != dList.end(); ++iter)
			{
				rxDevice* m_device = *iter;
				device[DeviceCount] = m_robotController->findDevice(m_device->id());
				DeviceCount++;

				/*if (m_device->userType() == 18)
				{
					m_DepthView = new DepthView;
					m_DepthView->Create(DepthView::IDD,this);
					m_DepthView->ShowWindow(SW_SHOW);
					m_DepthView->InitData(m_device);
				}*/

			}
			rData.subCount[robotCount] = DeviceCount;
		}
	}
	rData.subCount[0] = 0;
	rData.subCount[3] = DeviceCount;


	m_SimulatorDebug->RobotCount = robotCount;
	rData.rCount = robotCount;
	rData.dCount = DeviceCount;
	if (robotCount > 0)
	{
		m_SimulatorDebug->simDebugInit(sys,device);
		m_RobotControlData->initRobotControlData(sys,device,robotCount,rData.subCount);
		UpdateData(FALSE);
	}


}

void CKETI_SimulatorDlg::OnBnClickedStopSimBtn()
{
	// TODO: Add your control notification handler code here
	_btnStartSimulation.EnableWindow(TRUE);
	_btnStopSimulation.EnableWindow(FALSE);

	if (m_simulator)	{
		EnterCriticalSection(&_hCriticalSection);
		m_simulator->deactivate();
		LeaveCriticalSection(&_hCriticalSection);
	}
}

LRESULT CKETI_SimulatorDlg::OnLogMessage(WPARAM wParam, LPARAM lParam)
{
	//_logMessage.SetWindowText((char*)wParam);
	int iLen = m_logMsg.GetWindowTextLength();
	m_logMsg.SetSel(iLen, iLen);
	m_logMsg.ReplaceSel((TCHAR*)wParam);
	return 0;
}

/*	OnBnClickedClearBtn

	추가: 김영중
		모든 모델들을 언로드하고 화면을 클리어(관련 데이터 클리어)
*/
void CKETI_SimulatorDlg::OnBnClickedClearBtn()
{
	// TODO: Add your control notification handler code here

	OnBnClickedUnloadModelBtn();
	OnBnClickedStopSimBtn();

	m_SimulatorDebug->CloseDialog();
	m_debugTab.DeleteAllItems();


	m_modelListCtrl.DeleteAllItems();

	map<int, sItemInfo>::const_iterator	itr	=	m_itemInfoes.begin();
	for	(; itr != m_itemInfoes.end(); ++itr)	{
		
		if	(itr->second.is_system)
			m_simulator->unloadModel(itr->second.model_id);
		else
			m_simulator->unloadEnv(itr->second.model_id);
	}

	m_simulator->clearDisplay();
	
	m_itemInfoes.clear();

	_aml_loaded	=	false;
	_eml_loaded	=	false;

	//	reset 호출후 update 메서드에서 에러!
	m_simulator->reset();
}

/*	OnBnClickedResetBtn
	
	추가:	김영중
		시뮬레이션중, 모델을 초기위치로 재위치시킨다.
		(초기위치는 모델 로드시 주어진 x,y,z 좌표)
*/
void CKETI_SimulatorDlg::OnBnClickedResetBtn()
{
	// TODO: Add your control notification handler code here
	
	/*OnBnClickedStopSimBtn();
	OnBnClickedClearBtn();

	m_simulator->reset();*/

	map<int, sItemInfo>::const_iterator	itr	=	m_itemInfoes.begin();
	for	(; itr != m_itemInfoes.end(); ++itr)	{
		
		if	(itr->second.is_system)	{
			//HTransform T0(Rotation(), Displacement(0.f, 0.f, 0.2f));
			//m_simulator->T(itr->second.model_id, T0);
			m_simulator->T(itr->second.model_id, _transform_org);	//	초기 위치 재적용
			break	;
		}
	}
}

void CKETI_SimulatorDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	m_simulationController->stop();
	m_robotController->stop();
	m_oprosEngineRedirection->m_redir.KillProcess(EngineName);
	

	m_bRunSimulation	=	false;
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&_hCriticalSection);

	CDialog::OnClose();
}
void CKETI_SimulatorDlg::OnBnClickedEngineStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_oprosEngineRedirection->EngineStart();
}

void CKETI_SimulatorDlg::OnBnClickedEngineExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_oprosEngineRedirection->EngineExit();
}

void CKETI_SimulatorDlg::OnNMClickAppList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString sIndexValue,CompListName;

	if (pNMListView->iItem >= 0)
	{
		if (pNMListView->iSubItem == 0)
		{
			sIndexValue = AppList.GetItemText(pNMListView -> iItem , 0);
			AppList.DeleteColumn(1);
			CompListName = sIndexValue;
			CompListName += " Comp List";
			AppList.InsertColumn(1, CompListName ,LVCFMT_LEFT, 150 , 1);
			m_oprosEngineRedirection->viewComponentList(sIndexValue);
		}
	}
	
	UpdateData(FALSE);
	*pResult = 0;
}

void CKETI_SimulatorDlg::OnNMDblclkAppList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->iItem >= 0)
	{
		if (pNMListView->iSubItem == 0)
		{
			if (0 == m_oprosEngineRedirection->RunApp(pNMListView->iItem))
				MessageBox(
				TEXT("동일한 App이 실행중 입니다."),
				TEXT("OPRoS Engine Error"),
				MB_OK | MB_TOPMOST |MB_ICONWARNING);
		}
	}

	UpdateData(FALSE);
	*pResult = 0;
}

void CKETI_SimulatorDlg::OnLbnDblclkAppRunlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int AppRunListidx = AppRunList.GetCurSel();
	if (AppRunListidx >= 0)			m_oprosEngineRedirection->DelRunAppList(AppRunListidx);
}

void CKETI_SimulatorDlg::OnWheelrobotSimulatorcontrol()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_JoystickDlg = new CJoystickDlg;
	m_JoystickDlg->Create(CJoystickDlg::IDD,this);
	m_JoystickDlg->ShowWindow(SW_SHOW);
	m_JoystickDlg->SysDataInit(sys,device,rData);

}

void CKETI_SimulatorDlg::OnWheelrobotOprosenginecontrol()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_OPRoSJoystick = new COPRoSJoystickControl;
	m_OPRoSJoystick->Create(COPRoSJoystickControl::IDD,this);
	m_OPRoSJoystick->ShowWindow(SW_SHOW);
}

void CKETI_SimulatorDlg::OnManipulatorSimulatorcontrol()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_Manipulator = new	CManipulatorDlg;
	m_Manipulator->Create(CManipulatorDlg::IDD,this);
	m_Manipulator->ShowWindow(SW_SHOW);
	m_Manipulator->SysDataInit(sys,device,rData);
}

void CKETI_SimulatorDlg::OnManipulatorOprosenginecontrol()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_OPRoSManipulator = new COPRoSManipulatorDlg;
	m_OPRoSManipulator->Create(COPRoSManipulatorDlg::IDD, this);
	m_OPRoSManipulator->ShowWindow(SW_SHOW);
}

void CKETI_SimulatorDlg::OnFilePathsetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPathDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_oprosEngineRedirection->LoadAppList();		
	}
}

//void CKETI_SimulatorDlg::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	int itab = m_debugTab.GetCurSel();
//	m_SimulatorDebug->TcnSelected(itab);
//	*pResult = 0;
//}
//
//void CKETI_SimulatorDlg::OnTcnSelchangingTab2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	int itab = m_debugTab.GetCurSel();
//	m_SimulatorDebug->TcnSelecting(itab);
//	*pResult = 0;
//}

void CKETI_SimulatorDlg::OnTcnSelchangeDebugtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int itab = m_debugTab.GetCurSel();
	m_SimulatorDebug->TcnSelected(itab);
	*pResult = 0;
}

void CKETI_SimulatorDlg::OnTcnSelchangingDebugtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int itab = m_debugTab.GetCurSel();
	m_SimulatorDebug->TcnSelected(itab);
	*pResult = 0;
}

void CKETI_SimulatorDlg::OnNMClickTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_SimulatorDebug->viewCheckBoxCtl();
	*pResult = 0;
}

void CKETI_SimulatorDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	m_simulationController->stop();
	m_robotController->stop();
	m_oprosEngineRedirection->m_redir.KillProcess(EngineName);
	

	m_bRunSimulation	=	false;
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&_hCriticalSection);

}

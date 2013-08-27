// OPRoSJoystickControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"
#include "OPRoSJoystickControl.h"
#include "OPRoSEngineRedirection.h"
#include "dataType.h"

class OPRoSEngineRedirection;

// COPRoSJoystickControl 대화 상자입니다.

IMPLEMENT_DYNAMIC(COPRoSJoystickControl, CDialog)

COPRoSJoystickControl::COPRoSJoystickControl(CWnd* pParent /*=NULL*/)
	: CDialog(COPRoSJoystickControl::IDD, pParent)
	, Vforward(0)
	, Vangular(0)
{

}

COPRoSJoystickControl::~COPRoSJoystickControl()
{
}

void COPRoSJoystickControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Vforward);
	DDX_Text(pDX, IDC_EDIT2, Vangular);
	DDX_Control(pDX, IDC_COMBO1, JoystickAppList);
}


BEGIN_MESSAGE_MAP(COPRoSJoystickControl, CDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &COPRoSJoystickControl::OnBnClickedStart)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

void COPRoSJoystickControl::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}


BOOL COPRoSJoystickControl::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_joystick.SubclassDlgItem(IDC_JOYSTICK, this);
	m_joystick.SetFocus();
	JoystickAppList.InsertString(0,_T("SimulatorWheelControl1"));
	JoystickAppList.InsertString(1,_T("SimulatorWheelControl2"));
	JoystickAppList.InsertString(2,_T("SimulatorWheelControl3"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COPRoSJoystickControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	ShareJoystickStruct* pShareJoystickStruct = (ShareJoystickStruct*)m_pSharedMemory;
	
	CPoint moveDir = m_joystick.GetMoveDir();
	Vforward = (float)-(moveDir.y - 1) * 0.025;
	Vangular = (float)(moveDir.x - 1) * 0.025;
	pShareJoystickStruct->ForwardVelocity = Vforward;
	pShareJoystickStruct->AngularVelocity = Vangular;
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

void COPRoSJoystickControl::OnDestroy()
{
	CDialog::OnDestroy();
	if(m_pSharedMemory)
	UnmapViewOfFile(m_pSharedMemory);
	if(m_hMap)
	CloseHandle(m_hMap);

}

BOOL COPRoSJoystickControl::InitSharedMemory(CString appname)
{
	m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
									NULL,
									PAGE_READWRITE,
									0,
									sizeof(ShareJoystickStruct),
									appname);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		m_hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,
									appname);
	}

	if(m_hMap == NULL)
	{
		return FALSE;
	}

	m_pSharedMemory = (BYTE*)::MapViewOfFile(m_hMap,
											FILE_MAP_ALL_ACCESS,
											0,
											0,
											sizeof(ShareJoystickStruct));
	if(m_pSharedMemory == NULL)
	{
		return FALSE;
	}

	return TRUE;
}
void COPRoSJoystickControl::OnBnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString Appname;
	switch (JoystickAppList.GetCurSel())
	{
	case 0:
		InitSharedMemory(_T("SimulatorWheelControl1"));
		for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemCount(); i++)
		{
			if ("SimulatorWheelControl1" == ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemText(i,0))
			{
				((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->RunApp(i);
				appname = _T("SimulatorWheelControl1");
				break;
			}
		}
		break;

	case 1:
		InitSharedMemory(_T("SimulatorWheelControl2"));
		for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemCount(); i++)
		{
			if ("SimulatorWheelControl2" == ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemText(i,0))
			{
				((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->RunApp(i);
				appname = _T("SimulatorWheelControl2");
				break;
			}
		}
		break;

	case 2:
		InitSharedMemory(_T("SimulatorWheelControl3"));
		for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemCount(); i++)
		{
			if ("SimulatorWheelControl3" == ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemText(i,0))
			{
				((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->RunApp(i);
				appname = _T("SimulatorWheelControl3");
				break;
			}
		}
		break;

	default:
		break;
	}

	SetTimer(11,200,0);
}

void COPRoSJoystickControl::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(11);
	if(m_pSharedMemory)
	UnmapViewOfFile(m_pSharedMemory);
	if(m_hMap)
	CloseHandle(m_hMap);

	for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppRunList.GetCount() ; i++)
	{
		CString runname;
		((CKETI_SimulatorDlg*)GetParent())->AppRunList.GetText(i,runname);
		if (appname == runname)
		{
			((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->DelRunAppList(i);
			break;
		}
	}

	CDialog::OnClose();
}

void COPRoSJoystickControl::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	delete this;
}

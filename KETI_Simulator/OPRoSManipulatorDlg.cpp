// OPRoSManipulatorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Simulator.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"
#include "OPRoSManipulatorDlg.h"
#include "OPRoSEngineRedirection.h"
#include "dataType.h"

class OPRoSEngineRedirection;


// COPRoSManipulatorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COPRoSManipulatorDlg, CDialog)

COPRoSManipulatorDlg::COPRoSManipulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COPRoSManipulatorDlg::IDD, pParent)
	, ELink1(0)
	, ELink2(0)
	, ELink3(0)
	, ELink4(0)
	, ELink5(0)
	, ELink6(0)
{

}

COPRoSManipulatorDlg::~COPRoSManipulatorDlg()
{
}

void COPRoSManipulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, SLink1);
	DDX_Control(pDX, IDC_SLIDER2, SLink2);
	DDX_Control(pDX, IDC_SLIDER3, SLink3);
	DDX_Control(pDX, IDC_SLIDER4, SLink4);
	DDX_Control(pDX, IDC_SLIDER5, SLink5);
	DDX_Control(pDX, IDC_SLIDER6, SLink6);
	DDX_Control(pDX, IDC_COMBO1, ComponentList);
	DDX_Text(pDX, IDC_EDIT1, ELink1);
	DDX_Text(pDX, IDC_EDIT2, ELink2);
	DDX_Text(pDX, IDC_EDIT3, ELink3);
	DDX_Text(pDX, IDC_EDIT4, ELink4);
	DDX_Text(pDX, IDC_EDIT5, ELink5);
	DDX_Text(pDX, IDC_EDIT6, ELink6);
}


BEGIN_MESSAGE_MAP(COPRoSManipulatorDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &COPRoSManipulatorDlg::OnBnClickedButton1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO1, &COPRoSManipulatorDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &COPRoSManipulatorDlg::OnBnClickedRadio2)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

void COPRoSManipulatorDlg::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}

// COPRoSManipulatorDlg 메시지 처리기입니다.

BOOL COPRoSManipulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SLink1.SetRange(0,360);
	SLink1.SetTicFreq(1);
	SLink1.SetPos(180);
	ELink1 = 0;

	SLink2.SetRange(0,360);
	SLink2.SetTicFreq(1);
	SLink2.SetPos(180);
	ELink2 = 0;

	SLink3.SetRange(0,360);
	SLink3.SetTicFreq(1);
	SLink3.SetPos(180);
	ELink3 = 0;

	SLink4.SetRange(0,360);
	SLink4.SetTicFreq(1);
	SLink4.SetPos(180);
	ELink4 = 0;

	SLink5.SetRange(0,360);
	SLink5.SetTicFreq(1);
	SLink5.SetPos(180);
	ELink5 = 0;

	SLink6.SetRange(0,360);
	SLink6.SetTicFreq(1);
	SLink6.SetPos(180);
	ELink6 = 0;

	ComponentList.InsertString(0,_T("SimulatorManipulator1"));
	ComponentList.InsertString(1,_T("SimulatorManipulator2"));
	ComponentList.InsertString(2,_T("SimulatorManipulator3"));

	((CButton*) GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	angleFlag = 1;
	runFlag = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL COPRoSManipulatorDlg::InitSharedMemory(CString appname)
{
	m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
									NULL,
									PAGE_READWRITE,
									0,
									sizeof(ShareManipulatorStruct),
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
											sizeof(ShareManipulatorStruct));
	if(m_pSharedMemory == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

void COPRoSManipulatorDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	runFlag = 1;

	switch (ComponentList.GetCurSel())
	{
	case 0:
		InitSharedMemory(_T("SimulatorManipulator1"));
		for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemCount(); i++)
		{
			if ("SimulatorManipulator1" == ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemText(i,0))
			{
				((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->RunApp(i);
				appname = _T("SimulatorManipulator1");
				break;
			}
		}
		break;

	case 1:
		InitSharedMemory(_T("SimulatorManipulator2"));
		for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemCount(); i++)
		{
			if ("SimulatorManipulator2" == ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemText(i,0))
			{
				((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->RunApp(i);
				appname = _T("SimulatorManipulator2");
				break;
			}
		}
		break;

	case 2:
		InitSharedMemory(_T("SimulatorManipulator3"));
		for (int i = 0 ; i < ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemCount(); i++)
		{
			if ("SimulatorManipulator3" == ((CKETI_SimulatorDlg*)GetParent())->AppList.GetItemText(i,0))
			{
				((CKETI_SimulatorDlg*)GetParent())->m_oprosEngineRedirection->RunApp(i);
				appname = _T("SimulatorManipulator3");
				break;
			}
		}
		break;

	default:
		break;
	}
}

void COPRoSManipulatorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ShareManipulatorStruct* pShareManipulatorStruct = (ShareManipulatorStruct*)m_pSharedMemory;

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	ELink1 = ( angleFlag ? SLink1.GetPos() - 180 : (SLink1.GetPos() * (3.141592/180)) - 3.141592 );
	ELink2 = ( angleFlag ? SLink2.GetPos() - 180 : (SLink2.GetPos() * (3.141592/180)) - 3.141592 );
	ELink3 = ( angleFlag ? SLink3.GetPos() - 180 : (SLink3.GetPos() * (3.141592/180)) - 3.141592 );
	ELink4 = ( angleFlag ? SLink4.GetPos() - 180 : (SLink4.GetPos() * (3.141592/180)) - 3.141592 );
	ELink5 = ( angleFlag ? SLink5.GetPos() - 180 : (SLink5.GetPos() * (3.141592/180)) - 3.141592 );
	ELink6 = ( angleFlag ? SLink6.GetPos() - 180 : (SLink6.GetPos() * (3.141592/180)) - 3.141592 );
	
	UpdateData(FALSE);

	pShareManipulatorStruct->Link1 = (float)(SLink1.GetPos() * (3.141592 /180) - 3.141592);
	pShareManipulatorStruct->Link2 = (float)(SLink2.GetPos() * (3.141592 /180) - 3.141592);
	pShareManipulatorStruct->Link3 = (float)(SLink3.GetPos() * (3.141592 /180) - 3.141592);
	pShareManipulatorStruct->Link4 = (float)(SLink4.GetPos() * (3.141592 /180) - 3.141592);
	pShareManipulatorStruct->Link5 = (float)(SLink5.GetPos() * (3.141592 /180) - 3.141592);
	pShareManipulatorStruct->Link6 = (float)(SLink6.GetPos() * (3.141592 /180) - 3.141592);
}

void COPRoSManipulatorDlg::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	angleFlag = 1;
	ELink1 = ( angleFlag ? SLink1.GetPos() - 180 : (SLink1.GetPos() * (3.141592/180)) - 3.141592 );
	ELink2 = ( angleFlag ? SLink2.GetPos() - 180 : (SLink2.GetPos() * (3.141592/180)) - 3.141592 );
	ELink3 = ( angleFlag ? SLink3.GetPos() - 180 : (SLink3.GetPos() * (3.141592/180)) - 3.141592 );
	ELink4 = ( angleFlag ? SLink4.GetPos() - 180 : (SLink4.GetPos() * (3.141592/180)) - 3.141592 );
	ELink5 = ( angleFlag ? SLink5.GetPos() - 180 : (SLink5.GetPos() * (3.141592/180)) - 3.141592 );
	ELink6 = ( angleFlag ? SLink6.GetPos() - 180 : (SLink6.GetPos() * (3.141592/180)) - 3.141592 );

	UpdateData(FALSE);
}

void COPRoSManipulatorDlg::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	angleFlag = 0;
	ELink1 = ( angleFlag ? SLink1.GetPos() - 180 : (SLink1.GetPos() * (3.141592/180)) - 3.141592 );
	ELink2 = ( angleFlag ? SLink2.GetPos() - 180 : (SLink2.GetPos() * (3.141592/180)) - 3.141592 );
	ELink3 = ( angleFlag ? SLink3.GetPos() - 180 : (SLink3.GetPos() * (3.141592/180)) - 3.141592 );
	ELink4 = ( angleFlag ? SLink4.GetPos() - 180 : (SLink4.GetPos() * (3.141592/180)) - 3.141592 );
	ELink5 = ( angleFlag ? SLink5.GetPos() - 180 : (SLink5.GetPos() * (3.141592/180)) - 3.141592 );
	ELink6 = ( angleFlag ? SLink6.GetPos() - 180 : (SLink6.GetPos() * (3.141592/180)) - 3.141592 );

	UpdateData(FALSE);
}

BOOL COPRoSManipulatorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	ShareManipulatorStruct* pShareManipulatorStruct = (ShareManipulatorStruct*)m_pSharedMemory;
	
	if (angleFlag == 1)
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink1 > 180 ) 
			{
				ELink1 = 180;
				UpdateData(FALSE);
			}
			else if( ELink1 < -180)
			{
				ELink1 = -180;
				UpdateData(FALSE);
			}
			SLink1.SetPos(ELink1 + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link1
					= (float)(SLink1.GetPos() * (3.141592 /180) - 3.141592);
				
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT2)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink2 > 180 ) 
			{
				ELink2 = 180;
				UpdateData(FALSE);
			}
			else if( ELink2 < -180)
			{
				ELink2 = -180;
				UpdateData(FALSE);
			}
			SLink2.SetPos(ELink2 + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link2
					= (float)(SLink2.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink3 > 180 ) 
			{
				ELink3 = 180;
				UpdateData(FALSE);
			}
			else if( ELink3 < -180)
			{
				ELink3 = -180;
				UpdateData(FALSE);
			}
			SLink3.SetPos(ELink3 + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link3
					= (float)(SLink3.GetPos() * (3.141592 /180) - 3.141592 );
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink4 > 180 ) 
			{
				ELink4 = 180;
				UpdateData(FALSE);
			}
			else if( ELink4 < -180)
			{
				ELink4 = -180;
				UpdateData(FALSE);
			}
			SLink4.SetPos(ELink4 + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link4
					= (float)(SLink4.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT5)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink5 > 180 ) 
			{
				ELink5 = 180;
				UpdateData(FALSE);
			}
			else if( ELink5 < -180)
			{
				ELink5 = -180;
				UpdateData(FALSE);
			}
			SLink5.SetPos(ELink5 + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link5
					= (float)(SLink5.GetPos() * (3.141592 /180) - 3.141592 );
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT6)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink6 > 180 ) 
			{
				ELink6 = 180;
				UpdateData(FALSE);
			}
			else if( ELink6 < -180)
			{
				ELink6 = -180;
				UpdateData(FALSE);
			}
			SLink6.SetPos(ELink6 + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link6
					= (float)(SLink6.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}
	}
	else
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink1 > 3.141592 ) 
			{
				ELink1 = 3.141592;
				UpdateData(FALSE);
			}
			else if( ELink1 < -3.141592)
			{
				ELink1 = -3.141592;
				UpdateData(FALSE);
			}
			SLink1.SetPos(ELink1 * (180/3.141592) + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link1
					= (float)(SLink1.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT2)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink2 > 3.141592 ) 
			{
				ELink2 = 3.141592;
				UpdateData(FALSE);
			}
			else if( ELink2 < -3.141592)
			{
				ELink2 = -3.141592;
				UpdateData(FALSE);
			}
			SLink2.SetPos(ELink2 * (180/3.141592) + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link2
					= (float)(SLink2.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink3 > 3.141592 ) 
			{
				ELink3 = 3.141592;
				UpdateData(FALSE);
			}
			else if( ELink3 < -3.141592)
			{
				ELink3 = -3.141592;
				UpdateData(FALSE);
			}
			SLink3.SetPos(ELink3 * (180/3.141592) + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link3
					= (float)(SLink3.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink4 > 3.141592 ) 
			{
				ELink4 = 3.141592;
				UpdateData(FALSE);
			}
			else if( ELink4 < -3.141592)
			{
				ELink4 = -3.141592;
				UpdateData(FALSE);
			}
			SLink4.SetPos(ELink4 * (180/3.141592) + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link4
					= (float)(SLink4.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT5)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink5 > 3.141592 ) 
			{
				ELink5 = 3.141592;
				UpdateData(FALSE);
			}
			else if( ELink5 < -3.141592)
			{
				ELink5 = -3.141592;
				UpdateData(FALSE);
			}
			SLink5.SetPos(ELink5 * (180/3.141592) + 180 );
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link5
					= (float)(SLink5.GetPos() * (3.141592 /180) -3.141592);
			}
			return FALSE;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT6)->m_hWnd 
			&& pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
			if (ELink6 > 3.141592 ) 
			{
				ELink6 = 3.141592;
				UpdateData(FALSE);
			}
			else if( ELink6 < -3.141592)
			{
				ELink6 = -3.141592;
				UpdateData(FALSE);
			}
			SLink6.SetPos(ELink6 * (180/3.141592) + 180);
			if (runFlag == 1)
			{
				pShareManipulatorStruct->Link6
					= (float)(SLink6.GetPos() * (3.141592 /180) - 3.141592);
			}
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void COPRoSManipulatorDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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

void COPRoSManipulatorDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	delete this;
}

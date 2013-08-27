// ManipulatorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Simulator.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"
#include "ManipulatorDlg.h"


// CManipulatorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManipulatorDlg, CDialog)

CManipulatorDlg::CManipulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManipulatorDlg::IDD, pParent)
	, ELink1(0)
	, ELink2(0)
	, ELink3(0)
	, ELink4(0)
	, ELink5(0)
	, ELink6(0)
	, DCount(0)
{

}

CManipulatorDlg::~CManipulatorDlg()
{
}

void CManipulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, SLink1);
	DDX_Control(pDX, IDC_SLIDER2, SLink2);
	DDX_Control(pDX, IDC_SLIDER3, SLink3);
	DDX_Control(pDX, IDC_SLIDER4, SLink4);
	DDX_Control(pDX, IDC_SLIDER5, SLink5);
	DDX_Control(pDX, IDC_SLIDER6, SLink6);
	DDX_Control(pDX, IDC_COMBO1, CLink1);
	DDX_Control(pDX, IDC_COMBO2, CLink2);
	DDX_Control(pDX, IDC_COMBO3, CLink3);
	DDX_Control(pDX, IDC_COMBO4, CLink4);
	DDX_Control(pDX, IDC_COMBO5, CLink5);
	DDX_Control(pDX, IDC_COMBO6, CLink6);
	DDX_Control(pDX, IDC_COMBO8, RobotName);
	DDX_Text(pDX, IDC_EDIT1, ELink1);
	DDX_Text(pDX, IDC_EDIT2, ELink2);
	DDX_Text(pDX, IDC_EDIT3, ELink3);
	DDX_Text(pDX, IDC_EDIT4, ELink4);
	DDX_Text(pDX, IDC_EDIT5, ELink5);
	DDX_Text(pDX, IDC_EDIT6, ELink6);
}


BEGIN_MESSAGE_MAP(CManipulatorDlg, CDialog)
	ON_CBN_CLOSEUP(IDC_COMBO8, &CManipulatorDlg::OnCbnCloseupCombo8)
	ON_BN_CLICKED(IDC_SCONTROL, &CManipulatorDlg::OnBnClickedScontrol)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_DEG, &CManipulatorDlg::OnBnClickedDeg)
	ON_BN_CLICKED(IDC_RAD, &CManipulatorDlg::OnBnClickedRad)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CManipulatorDlg 메시지 처리기입니다.

void CManipulatorDlg::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}

BOOL CManipulatorDlg::OnInitDialog()
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

	((CButton*) GetDlgItem(IDC_DEG))->SetCheck(TRUE);
	angleFlag = 1;
	runFlag = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CManipulatorDlg::SysDataInit(rxSystem** sys, rxDevice** device, RobotInfo rData)
{
	rInfo = rData;
	for (int i = 0 ; i < rData.rCount ; i++)
	{
		rSys[i] = sys[i];
		RobotName.InsertString(i,rSys[i]->name().c_str());
	}
	for (int i = 0 ; i < rData.dCount ; i++)
	{
		rDevice[i] = device[i];
	}
}

void CManipulatorDlg::OnCbnCloseupCombo8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CLink1.ResetContent();
	CLink2.ResetContent();
	CLink3.ResetContent();
	CLink4.ResetContent();
	CLink5.ResetContent();
	CLink6.ResetContent();

	int rSelect = RobotName.GetCurSel();
	CLink1.InsertString(0,_T("No Device"));
	CLink2.InsertString(0,_T("No Device"));
	CLink3.InsertString(0,_T("No Device"));
	CLink4.InsertString(0,_T("No Device"));
	CLink5.InsertString(0,_T("No Device"));
	CLink6.InsertString(0,_T("No Device"));


	for (int i = rInfo.subCount[rSelect] ; i < rInfo.subCount[rSelect+1] ; i++)
	{
		CLink1.InsertString(i-rInfo.subCount[rSelect]+1,rDevice[i]->name().c_str());
		CLink2.InsertString(i-rInfo.subCount[rSelect]+1,rDevice[i]->name().c_str());
		CLink3.InsertString(i-rInfo.subCount[rSelect]+1,rDevice[i]->name().c_str());
		CLink4.InsertString(i-rInfo.subCount[rSelect]+1,rDevice[i]->name().c_str());
		CLink5.InsertString(i-rInfo.subCount[rSelect]+1,rDevice[i]->name().c_str());
		CLink6.InsertString(i-rInfo.subCount[rSelect]+1,rDevice[i]->name().c_str());
	}

	CLink1.SetCurSel(0);
	CLink2.SetCurSel(0);
	CLink3.SetCurSel(0);
	CLink4.SetCurSel(0);
	CLink5.SetCurSel(0);
	CLink6.SetCurSel(0);
}

void CManipulatorDlg::OnBnClickedScontrol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int rSelect = RobotName.GetCurSel();
	int	link1	=	CLink1.GetCurSel();
	int	link2	=	CLink2.GetCurSel();
	int	link3	=	CLink3.GetCurSel();
	int	link4	=	CLink4.GetCurSel();
	int	link5	=	CLink5.GetCurSel();
	int	link6	=	CLink6.GetCurSel();

	int	count = 0;

	if (link1 > 0)
	{
		Link[count] = rInfo.subCount[rSelect] + link1 - 1;
		sliderNumber[count] = 1;
		count++;
	}

	if (link2 > 0)
	{
		Link[count] = rInfo.subCount[rSelect] + link2 - 1;
		sliderNumber[count] = 2;
		count++;
	}

	if (link3 > 0)
	{
		Link[count] = rInfo.subCount[rSelect] + link3 - 1;
		sliderNumber[count] = 3;
		count++;
	}

	if (link4 > 0)
	{
		Link[count] = rInfo.subCount[rSelect] + link4 - 1;
		sliderNumber[count] = 4;
		count++;
	}

	if (link5 > 0)
	{
		Link[count] = rInfo.subCount[rSelect] + link5 - 1;
		sliderNumber[count] = 5;
		count++;
	}

	if (link6 > 0)
	{
		Link[count] = rInfo.subCount[rSelect] + link6 - 1;
		sliderNumber[count] = 6;
		count++;
	}
	DCount = count;
	runFlag = 1;
	CLink1.EnableWindow(FALSE);
	CLink2.EnableWindow(FALSE);
	CLink3.EnableWindow(FALSE);
	CLink4.EnableWindow(FALSE);
	CLink5.EnableWindow(FALSE);
	CLink6.EnableWindow(FALSE);
}

void CManipulatorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	float buf[2];

	ELink1 = ( angleFlag ? SLink1.GetPos() - 180 : (SLink1.GetPos() * (3.141592/180)) - 3.141592 );
	ELink2 = ( angleFlag ? SLink2.GetPos() - 180 : (SLink2.GetPos() * (3.141592/180)) - 3.141592 );
	ELink3 = ( angleFlag ? SLink3.GetPos() - 180 : (SLink3.GetPos() * (3.141592/180)) - 3.141592 );
	ELink4 = ( angleFlag ? SLink4.GetPos() - 180 : (SLink4.GetPos() * (3.141592/180)) - 3.141592 );
	ELink5 = ( angleFlag ? SLink5.GetPos() - 180 : (SLink5.GetPos() * (3.141592/180)) - 3.141592 );
	ELink6 = ( angleFlag ? SLink6.GetPos() - 180 : (SLink6.GetPos() * (3.141592/180)) - 3.141592 );

	UpdateData(FALSE);

	if (runFlag)
	{
		for( int i = 0 ; i < DCount ; i++)
		{
			switch(sliderNumber[i])
			{
			case 1:
				buf[0] = (float)(SLink1.GetPos() * (3.141592 /180) - 3.141592);
				rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
				break;
			case 2:
				buf[0] = (float)(SLink2.GetPos() * (3.141592 /180) - 3.141592);
				rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
				break;
			case 3:
				buf[0] = (float)(SLink3.GetPos() * (3.141592 /180) - 3.141592);
				rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
				break;
			case 4:
				buf[0] = (float)(SLink4.GetPos() * (3.141592 /180) - 3.141592);
				rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
				break;
			case 5:
				buf[0] = (float)(SLink5.GetPos() * (3.141592 /180) - 3.141592);
				rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
				break;
			case 6:
				buf[0] = (float)(SLink6.GetPos() * (3.141592 /180) - 3.141592);
				rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
				break;
			default:
				break;
			}
		}
	}
}

void CManipulatorDlg::OnBnClickedDeg()
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

void CManipulatorDlg::OnBnClickedRad()
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

BOOL CManipulatorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
		float buf[2];

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
				buf[0] = (float)(SLink1.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 1)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink2.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 2)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink3.GetPos() * (3.141592 /180) - 3.141592 );
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 3)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink4.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 4)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink5.GetPos() * (3.141592 /180) - 3.141592 );
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 5)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink6.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 6)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink1.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 1)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink2.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 2)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink3.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 3)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink4.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 4)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink5.GetPos() * (3.141592 /180) -3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 5)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
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
				buf[0] = (float)(SLink6.GetPos() * (3.141592 /180) - 3.141592);
				for(int i = 0 ; i < DCount ; i++)
				{
					if (sliderNumber[i] == 6)
					{
						rDevice[Link[i]]->writeDeviceValue((void*)buf,sizeof(float)*2);
						break;
					}
				}
			}
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CManipulatorDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete this;
}

// View1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Simulator.h"
#include "KETI_Simulator.h"
#include "View1Dlg.h"
//#include "RobotController.h"
#include "InputCondition.h"



// CView1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CView1Dlg, CDialog)

CView1Dlg::CView1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CView1Dlg::IDD, pParent),
_simulator(NULL)
{


}

CView1Dlg::~CView1Dlg()
{
	/*if (m_RobotController)
		delete m_RobotController;*/

}

void CView1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICELIST, m_dlist);
}


BEGIN_MESSAGE_MAP(CView1Dlg, CDialog)
//	ON_WM_TIMER()
ON_WM_TIMER()
ON_NOTIFY(NM_DBLCLK, IDC_DEVICELIST, &CView1Dlg::OnNMDblclkDevicelist)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_DEVICELIST, &CView1Dlg::OnNMCustomdrawDevicelist)
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CView1Dlg 메시지 처리기입니다.

void CView1Dlg::TcnSelected(BOOL sel, rxSystem** sys, rxDevice** device)
{
	int DeviceCount = 0;
	if(sel)
	{
		Allview = true;
		m_dlist.InsertColumn(0, _T("RobotName"),LVCFMT_LEFT, 100, 0);
		m_dlist.InsertColumn(1, _T("DeviceName"),LVCFMT_LEFT, 100, 1);
		m_dlist.InsertColumn(2, _T("DeviceID"),LVCFMT_LEFT, 20, 2);
		m_dlist.InsertColumn(3, _T("Value"),LVCFMT_LEFT, 100, 3);
		m_dlist.InsertColumn(4, _T("Condition"),LVCFMT_LEFT, 100, 4);

	}
	else
	{
		Allview = false;
		m_dlist.InsertColumn(0, _T("DeviceName"),LVCFMT_LEFT, 70, 0);
		m_dlist.InsertColumn(1, _T("DeviceID"),LVCFMT_LEFT, 30, 1);
		m_dlist.InsertColumn(2, _T("Value"),LVCFMT_LEFT, 100, 2);
		m_dlist.InsertColumn(3, _T("Condition"),LVCFMT_LEFT, 100, 3);
		DeviceList& dList = sys[0]->getDevices();
		for(DeviceList::iterator iter = dList.begin() ; iter != dList.end(); ++iter)
		{
			rxDevice* device = *iter;
			DeviceInfo[DeviceCount].RobotID = m_robotID;
			DeviceInfo[DeviceCount].DeviceID = device->id();
			DeviceInfo[DeviceCount].DeviceName = device->name().c_str();
			DeviceCount++;
		}
	}
	for(int i = 0 ; i < DeviceCount ; i++)
	{
		rdevice[i] = device[i];
	}
	m_dlist.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	SetTimer(1,200,0);
	UpdateData(FALSE);
}

void CView1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (Allview)
	{
	}

	else
	{
		//m_dlist.DeleteAllItems();
		for (int i = 0 ; i <subCount ; i++)
		{
			CString dC_buffer;
			CString ID;
			char d_buffer[512];
			float deviceValue[10000] ={0,};

			unsigned char vision[320*240*3]={0,};

			int type = rdevice[i]->type();
			int cnt;

			if (type == 1)
				cnt = rdevice[i]->readDeviceValue((void*)deviceValue,sizeof(float)*10000)/4;
			else if(type == 2)
				cnt = rdevice[i]->monitorDeviceValue((void*)deviceValue,sizeof(float)*10000)/4;

			//if (rdevice[i]->userType() == 18)
			//{
			//	cnt = rdevice[i]->readDeviceValue((void*)vision,sizeof(float)*320*240*3);
			//	for (int j = 0 ; j < cnt ; j++)
			//	{
			//		sprintf_s(d_buffer,"%d ",vision[j]);
			//		dC_buffer +=  d_buffer;
			//	}
			//	m_dlist.SetItemText(i,0,DeviceInfo[i].DeviceName);
			//	//m_dlist.InsertItem(i,DeviceInfo[i].DeviceName);
			//	ID.Format(_T("%d"),DeviceInfo[i].DeviceID);
			//	m_dlist.SetItemText(i,1,ID);
			//	m_dlist.SetItemText(i,2,dC_buffer);
			//}
			for (int j = 0 ; j < cnt ; j++)
			{
				sprintf_s(d_buffer,"%1.4f ",deviceValue[j]);
				dC_buffer +=  d_buffer;
				//TRACE("%f",deviceValue[j]);
			}
			m_dlist.SetItemText(i,0,DeviceInfo[i].DeviceName);
			//m_dlist.InsertItem(i,DeviceInfo[i].DeviceName);
			ID.Format(_T("%d"),DeviceInfo[i].DeviceID);
			m_dlist.SetItemText(i,1,ID);
			m_dlist.SetItemText(i,2,dC_buffer);
		}
		CString buff;
		for (int i = 0 ; i < CdataCount ; i++)
		{
			switch(Cdata[i].Condition)
			{
			case 0:
				buff.Format(_T("== %f"),Cdata[i].Value);
				m_dlist.SetItemText(Cdata[i].ListNumber,3,buff);
				break;
			case 1:
				buff.Format(_T("> %f"),Cdata[i].Value);
				m_dlist.SetItemText(Cdata[i].ListNumber,3,buff);
				break;
			case 2:
				buff.Format(_T(">= %f"),Cdata[i].Value);
				m_dlist.SetItemText(Cdata[i].ListNumber,3,buff);
				break;
			case 3:
				buff.Format(_T("< %f"),Cdata[i].Value);
				m_dlist.SetItemText(Cdata[i].ListNumber,3,buff);
				break;
			case 4:
				buff.Format(_T("<= %f"),Cdata[i].Value);
				m_dlist.SetItemText(Cdata[i].ListNumber,3,buff);
				break;
			case 5:
				buff.Format(_T("!= %f"),Cdata[i].Value);
				m_dlist.SetItemText(Cdata[i].ListNumber,3,buff);
				break;
			default:
				break;
			}
		}

	}

	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

BOOL CView1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//_simulator->initialize();

	/*m_RobotController = new RobotController();
	m_RobotController->registerSimulator(_simulator);*/

	CdataCount = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CView1Dlg::OnNMDblclkDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	char buf[128];
	CString buff;

	bool flag = 1;
	if (pNMListView->iItem >= 0)
	{
		CInputCondition	dlg;
		if (dlg.DoModal() == IDOK)
		{
			for (int i = 0 ; i < CdataCount ; i++)
			{
				if (Cdata[i].ListNumber == pNMListView->iItem)
				{
					Cdata[i].Value = dlg.Value;
					Cdata[i].Condition = dlg.count;
					flag = 0;
					break;
				}
			}
			if (flag)
			{
				Cdata[CdataCount].Value = dlg.Value;
				Cdata[CdataCount].Condition = dlg.count;
				Cdata[CdataCount].ListNumber = pNMListView->iItem;
				CdataCount++;
			}
			switch(dlg.count)
			{
			case 0:
				sprintf_s(buf,"== %f",dlg.Value);
				buff = buf;
				m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 1:
				sprintf_s(buf,"> %f",dlg.Value);
				buff = buf;
				m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 2:
				sprintf_s(buf,">= %f",dlg.Value);
				buff = buf;
				m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 3:
				sprintf_s(buf,"< %f",dlg.Value);
				buff = buf;
				m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 4:
				sprintf_s(buf,"<= %f",dlg.Value);
				buff = buf;
				m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 5:
				sprintf_s(buf,"!= %f",dlg.Value);
				buff = buf;
				m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			default:
				break;
			}
		}
	}
	*pResult = 0;
}

void CView1Dlg::OnNMCustomdrawDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    *pResult = 0;

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF crText;
		for (int i = 0 ; i < CdataCount ; i++)
		{
			float deviceValue[65] ={0,};
			int cnt;
			if ( pLVCD->nmcd.dwItemSpec == Cdata[i].ListNumber)
			{
				int type = rdevice[Cdata[i].ListNumber]->type();
				if (type == 1)
					rdevice[Cdata[i].ListNumber]->readDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
				else if (type == 2)
					rdevice[Cdata[i].ListNumber]->monitorDeviceValue((void*)deviceValue,sizeof(float)*100)/4;

				switch(Cdata[i].Condition)
				{
				case 0:
					if ( deviceValue[0] == Cdata[i].Value)
						crText = RGB(255,0,0);
					else
						crText = RGB(255,255,255);
					pLVCD->clrTextBk = crText;
					return;
					break;
				case 1:
					if ( deviceValue[0] > Cdata[i].Value)
						crText = RGB(255,0,0);
					else
						crText = RGB(255,255,255);
					pLVCD->clrTextBk = crText;
					return;
					break;
				case 2:
					if ( deviceValue[0] >= Cdata[i].Value)
						crText = RGB(255,0,0);
					else
						crText = RGB(255,255,255);
					pLVCD->clrTextBk = crText;
					return;
					break;
				case 3:
					if ( deviceValue[0] < Cdata[i].Value)
						crText = RGB(255,0,0);
					else
						crText = RGB(255,255,255);
					pLVCD->clrTextBk = crText;
					return;
					break;
				case 4:
					if ( deviceValue[0] <= Cdata[i].Value)
						crText = RGB(255,0,0);
					else
						crText = RGB(255,255,255);
					pLVCD->clrTextBk = crText;
					return;
					break;
				case 5:
					if ( deviceValue[0] != Cdata[i].Value)
						crText = RGB(255,0,0);
					else
						crText = RGB(255,255,255);
					pLVCD->clrTextBk = crText;
					return;
					break;
				default :
					break;
				}
			}
			else
				crText = RGB(255,255,255);
		}
		crText = RGB(255,255,255);
		pLVCD->clrTextBk = crText;
		//*pResult = CDRF_DODEFAULT;
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else
	{
		*pResult = CDRF_DODEFAULT;
	}
}

void CView1Dlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();

	CloseWindow();
	KillTimer(1);	
}

// View2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Simulator.h"
#include "KETI_Simulator.h"
#include "View2Dlg.h"
//#include "RobotController.h"
#include "InputCondition.h"


// CView2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CView2Dlg, CDialog)

CView2Dlg::CView2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CView2Dlg::IDD, pParent)
{
	

}

CView2Dlg::~CView2Dlg()
{
	/*if (m_RobotController)
		delete m_RobotController;
	delete	[]m_data;*/
}

void CView2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICELIST, m_dlist);
}


BEGIN_MESSAGE_MAP(CView2Dlg, CDialog)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_DEVICELIST, &CView2Dlg::OnNMDblclkDevicelist)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DEVICELIST, &CView2Dlg::OnNMCustomdrawDevicelist)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CView2Dlg 메시지 처리기입니다.

void CView2Dlg::TcnSelected(BOOL sel, rxSystem** sys, rxDevice** device)
{
	int DeviceCount = 0;

	if(sel)
	{
		Allview = true;
		m_dlist.InsertColumn(0, _T("RobotName"),LVCFMT_LEFT, 50, 0);
		m_dlist.InsertColumn(1, _T("DeviceName"),LVCFMT_LEFT, 70, 1);
		m_dlist.InsertColumn(2, _T("DeviceID"),LVCFMT_LEFT, 30, 2);
		m_dlist.InsertColumn(3, _T("Value"),LVCFMT_LEFT, 100, 3);
		m_dlist.InsertColumn(4, _T("Condition"),LVCFMT_LEFT, 80, 4);
		m_dlist.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
		for (int i = 0 ; i < allCount ; i++)
		{
			rdevice[i] = device[i];
		}
	}
	else
	{
		Allview = false;
		m_dlist.InsertColumn(0, _T("DeviceName"),LVCFMT_LEFT, 70, 0);
		m_dlist.InsertColumn(1, _T("DeviceID"),LVCFMT_LEFT, 30, 1);
		m_dlist.InsertColumn(2, _T("Value"),LVCFMT_LEFT, 100, 2);
		m_dlist.InsertColumn(3, _T("Condition"),LVCFMT_LEFT, 100, 3);
		DeviceList& dList = sys[1]->getDevices();
		for(DeviceList::iterator iter = dList.begin() ; iter != dList.end(); ++iter)
		{
			rxDevice* device = *iter;
			DeviceInfo[DeviceCount].RobotID = m_robotID;
			DeviceInfo[DeviceCount].DeviceID = device->id();
			DeviceInfo[DeviceCount].DeviceName = device->name().c_str();
			DeviceCount++;
		}
		for(int  i = preCount ; i < DeviceCount+preCount ; i++)
		{
			rdevice[i] = device[i];
		}
	}
	m_dlist.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	SetTimer(2,200,0);

	UpdateData(FALSE);
}
void CView2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	float deviceValue[128] = {0,};
	CString buff;
	
	if (Allview)
	{
		for (int i = 0 ; i <DeviceCount ; i++)
		{
			CString dC_buffer;
			char d_buffer[1024];

			for (int j = 0 ; j < allCount ; j++)
			{
				if (rdevice[j]->id() == m_data[i].DeviceID)
				{
					int type = rdevice[j]->type();
					int cnt;

					if (type == 1)
						cnt = rdevice[j]->readDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
					else if(type == 2)
						cnt = rdevice[j]->monitorDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
					for (int j = 0 ; j < cnt ; j++)
					{
						sprintf_s(d_buffer,"%1.4f ",deviceValue[j]);
						dC_buffer +=  d_buffer;
					}
					break;
				}
			}
			m_dlist.SetItemText(i,3,dC_buffer);
			buff = m_dlist.GetItemText(i,0);
			m_dlist.SetItemText(i,0,buff);
			buff = m_dlist.GetItemText(i,1);
			m_dlist.SetItemText(i,1,buff);
			buff = m_dlist.GetItemText(i,2);
			m_dlist.SetItemText(i,2,buff);
		}
	}

	else
	{
		m_dlist.DeleteAllItems();
		for (int i = 0 ; i <subCount ; i++)
		{
			CString dC_buffer;
			CString ID;
			char d_buffer[1024];

			int type = rdevice[i+preCount]->type();
			int cnt;

			if (type == 1)
				cnt = rdevice[i+preCount]->readDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
			else if(type == 2)
				cnt = rdevice[i+preCount]->monitorDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
			
			for (int j = 0 ; j < cnt ; j++)
			{
				sprintf_s(d_buffer,"%1.4f ",deviceValue[j]);
				dC_buffer +=  d_buffer;
			}
			m_dlist.InsertItem(i,DeviceInfo[i].DeviceName);
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

BOOL CView2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	/*m_RobotController = new RobotController();
	m_RobotController->registerSimulator(_simulator);*/
	CdataCount = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CView2Dlg::GetDeviceList(struct Vdata* data, int listCount)
{
	CString buf;
	int C_count = 0;
	m_dlist.DeleteAllItems();
	m_data = new struct Vdata [listCount];
	m_data = data;
	for (int i = 0 ; i < listCount ; i++)
	{
		m_data[i].DeviceID		= data[i].DeviceID;
		m_data[i].RobotID		= data[i].RobotID;
		m_dlist.InsertItem(i,m_data[i].RobotName);
		m_dlist.SetItemText(i,1,m_data[i].DeviceName);
		buf.Format(_T("%d"),m_data[i].DeviceID);
		m_dlist.SetItemText(i,2,buf);
		for (int j = 0 ; j < CdataCount ; j++)
		{
			if (m_data[i].DeviceID == Cdata[j].ListNumber)
			{
				switch(Cdata[j].Condition)
				{
				case 0:
					buf.Format(_T("== %f"),Cdata[i].Value);
					m_dlist.SetItemText(i,4,buf);
					break;
				case 1:
					buf.Format(_T("> %f"),Cdata[i].Value);
					m_dlist.SetItemText(i,4,buf);
					break;
				case 2:
					buf.Format(_T(">= %f"),Cdata[i].Value);
					m_dlist.SetItemText(i,4,buf);
					break;
				case 3:
					buf.Format(_T("< %f"),Cdata[i].Value);
					m_dlist.SetItemText(i,4,buf);
					break;
				case 4:
					buf.Format(_T("<= %f"),Cdata[i].Value);
					m_dlist.SetItemText(i,4,buf);
					break;
				case 5:
					buf.Format(_T("!= %f"),Cdata[i].Value);
					m_dlist.SetItemText(i,4,buf);
					break;
				default:
					break;
				}
				C_count++;
				break;
			}
		}
	}
	CdataCount = C_count;
	DeviceCount = listCount;
}

void CView2Dlg::OnNMDblclkDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString buff;

	bool flag = 1;
	if (pNMListView->iItem >= 0)
	{
		CInputCondition	dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (Allview)
			{
				for (int i = 0 ; i <CdataCount ; i++)
				{
					if (Cdata[i].ListNumber == _ttoi(m_dlist.GetItemText(pNMListView->iItem,2)))
					{
						Cdata[i].Value = dlg.Value;
						Cdata[i].Condition = dlg.count;
						Cdata[i].ListNumber = _ttoi(m_dlist.GetItemText(pNMListView->iItem,2));
						flag  = 0;
						//break;
					}
				}
					
				if (flag)
				{
					Cdata[CdataCount].Value = dlg.Value;
					Cdata[CdataCount].Condition = dlg.count;
					Cdata[CdataCount].ListNumber = m_data[pNMListView->iItem].DeviceID;
					CdataCount++;
				}
			}
			else
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
			}
			switch(dlg.count)
			{
			case 0:
				buff.Format(_T("== %f"),dlg.Value);
				if (Allview)
					m_dlist.SetItemText(pNMListView->iItem,4,buff);
				else
					m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 1:
				buff.Format(_T("> %f"),dlg.Value);
				if (Allview)
					m_dlist.SetItemText(pNMListView->iItem,4,buff);
				else
					m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 2:
				buff.Format(_T(">= %f"),dlg.Value);
				if (Allview)
					m_dlist.SetItemText(pNMListView->iItem,4,buff);
				else
					m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 3:
				buff.Format(_T("< %f"),dlg.Value);
				if (Allview)
					m_dlist.SetItemText(pNMListView->iItem,4,buff);
				else
					m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 4:
				buff.Format(_T("<= %f"),dlg.Value);
				if (Allview)
					m_dlist.SetItemText(pNMListView->iItem,4,buff);
				else
					m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			case 5:
				buff.Format(_T("!= %f"),dlg.Value);
				if (Allview)
					m_dlist.SetItemText(pNMListView->iItem,4,buff);
				else
					m_dlist.SetItemText(pNMListView->iItem,3,buff);
				break;
			default:
				break;
			}
		}
	}
	*pResult = 0;
}

void CView2Dlg::OnNMCustomdrawDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
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
			if (Allview)
			{
				float deviceValue[65] ={0,};
				//int cnt;
				for(int j = 0 ;  j < DeviceCount ; j++)
				{
					if (pLVCD->nmcd.dwItemSpec == j)
					{
						if (Cdata[i].ListNumber == m_data[j].DeviceID)
						{
							for (int k = 0 ; k < allCount ; k++)
							{
								if (m_data[j].DeviceID ==rdevice[k]->id())
								{
									int type = rdevice[k]->type();
									if (type == 1)
										int cnt = rdevice[k]->readDeviceValue((void*)deviceValue,sizeof(float)*100);
									else if (type == 2)
										int cnt = rdevice[k]->monitorDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
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
									break;
								}
							}
						}
					}
				}
							
			}

			else
			{

				float deviceValue[65] ={0,};
				int cnt;
				if ( pLVCD->nmcd.dwItemSpec == Cdata[i].ListNumber)
				{
					int type = rdevice[Cdata[i].ListNumber+preCount]->type();
					if (type == 1)
						cnt = rdevice[Cdata[i].ListNumber+preCount]->readDeviceValue((void*)deviceValue,sizeof(float)*100)/4;
					else if (type == 2)
						cnt = rdevice[Cdata[i].ListNumber+preCount]->monitorDeviceValue((void*)deviceValue,sizeof(float)*100)/4;

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
		}
		//crText = RGB(255,255,255);
		//pLVCD->clrTextBk = crText;
		*pResult = CDRF_DODEFAULT;
	}
}

void CView2Dlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();

	CloseWindow();
	KillTimer(2);
}

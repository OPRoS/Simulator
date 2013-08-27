#include "StdAfx.h"
#include "Simulator.h"
#include "SimulatorDebug.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"

int			subDeviceCount[3];

CSimulatorDebug::CSimulatorDebug(void)
{
}

CSimulatorDebug::~CSimulatorDebug(void)
{
}

void CSimulatorDebug::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}

void CSimulatorDebug::simDebugInit(rxSystem** sys, rxDevice** device)
{

	DeviceCount = 0;
	ListCount = 0;
	for (int i = 0 ; i < RobotCount ; i++)
	{
		subDeviceCount[i] = 0;
		DeviceList& dList = sys[i]->getDevices();
		for(DeviceList::iterator iter = dList.begin() ; iter != dList.end(); ++iter)
		{
			rxDevice* device = *iter;
			DeviceInfoData[DeviceCount].RobotID = RobotID[i];
			DeviceInfoData[DeviceCount].DevieID = device->id();
			DeviceInfoData[DeviceCount].DeviceName = device->name().c_str();
			DeviceCount++;
			subDeviceCount[i]++;
		}
	}
	getTreeItem(sys,device);
}

void CSimulatorDebug::getTreeItem(rxSystem** sys, rxDevice** device)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	CRect	Rect;
	pMain->m_debugTab.GetClientRect(&Rect);
	pMain->m_debugTab.DeleteAllItems();
	pMain->m_deviceTree.DeleteAllItems();

	HTREEITEM	root = 
		pMain->m_deviceTree.InsertItem(_T("RobotList"), 0, 0, TVI_ROOT, TVI_LAST);

	for (int i = 0 ; i < RobotCount ; i++)
	{
		RobotNode[i] = 
			pMain->m_deviceTree.InsertItem(RobotName[i] , 1, 1, root, TVI_LAST);

		pMain->m_debugTab.InsertItem(i+1,RobotName[i]);
		SetTabgen(i);
	}
	pMain->m_debugTab.InsertItem(RobotCount,_T("All Robot View"));
	SetTabgen(RobotCount);

	pMain->m_debugTab.SetCurSel(0);
	m_view1.ShowWindow(SW_SHOW);
	m_pwndShow = &m_view1;

	int preRobotID = DeviceInfoData[0].RobotID;
	int ArrCount = 0;
	for (int i = 0; i < DeviceCount ; i++)
	{
		if (DeviceInfoData[i].RobotID != preRobotID)	ArrCount++;
		deviceNode[i] = 
			pMain->m_deviceTree.InsertItem
			(DeviceInfoData[i].DeviceName, 2, 2, RobotNode[ArrCount],TVI_LAST);
		preRobotID = DeviceInfoData[i].RobotID;
	}

	dlistColumn(RobotCount,sys,device);	
}

void CSimulatorDebug::CloseDialog(void)
{
	//m_view1.CloseWindow();	
	switch(RobotCount)
	{
	case 1:
		m_view1.OnClose();
		m_view1.DestroyWindow();
		m_view2.OnClose();
		m_view2.DestroyWindow();
		break;
	case 2:
		m_view1.OnClose();
		m_view1.DestroyWindow();
		m_view2.OnClose();
		m_view2.DestroyWindow();
		m_view3.OnClose();
		m_view3.DestroyWindow();
		break;
	case 3:
		m_view1.OnClose();
		m_view1.DestroyWindow();
		m_view2.OnClose();
		m_view2.DestroyWindow();
		m_view3.OnClose();
		m_view3.DestroyWindow();
		m_view4.OnClose();
		m_view4.DestroyWindow();
		break;
	default:
		break;
	}
}


void CSimulatorDebug::SetTabgen(int viewNumber)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	CRect	Rect;
	pMain->m_debugTab.GetClientRect(&Rect);

	switch(viewNumber)
	{
	case 0:
		m_view1.Create(IDD_VIEW1,&pMain->m_debugTab);
		m_view1.SetWindowPos(NULL,2,22,Rect.Width()-5,Rect.Height()-25,SWP_SHOWWINDOW|SWP_NOZORDER);
		m_pwndShow = &m_view1;
		m_view1.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_view2.Create(IDD_VIEW2,&pMain->m_debugTab);
		m_view2.SetWindowPos(NULL,2,22,Rect.Width()-5,Rect.Height()-25,SWP_SHOWWINDOW|SWP_NOZORDER);
		m_pwndShow = &m_view2;
		m_view2.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_view3.Create(IDD_VIEW3,&pMain->m_debugTab);
		m_view3.SetWindowPos(NULL,2,22,Rect.Width()-5,Rect.Height()-25,SWP_SHOWWINDOW|SWP_NOZORDER);
		m_pwndShow = &m_view3;
		m_view3.ShowWindow(SW_HIDE);
		break;

	case 3:
		m_view4.Create(IDD_VIEW4,&pMain->m_debugTab);
		m_view4.SetWindowPos(NULL,2,22,Rect.Width()-5,Rect.Height()-25,SWP_SHOWWINDOW|SWP_NOZORDER);
		m_pwndShow = &m_view4;
		m_view4.ShowWindow(SW_HIDE);
		break;

	default:
		break;
	}
}

void CSimulatorDebug::TcnSelected(int itab)
{
	if (m_pwndShow != NULL)
	{
		m_pwndShow->ShowWindow(SW_HIDE);
		m_pwndShow = NULL;
	}

	switch(itab)
	{
	case 0:
		m_view1.ShowWindow(SW_SHOW);
		m_pwndShow = &m_view1;
		break;
	case 1:
		m_view2.ShowWindow(SW_SHOW);
		m_pwndShow = &m_view2;
		break;
	case 2:
		m_view3.ShowWindow(SW_SHOW);
		m_pwndShow = &m_view3;
		break;
	case 3:
		m_view4.ShowWindow(SW_SHOW);
		m_pwndShow = &m_view4;
		break;
	default:
		break;
	}
}

void CSimulatorDebug::TcnSelecting(int itab)
{
	if (m_pwndShow != NULL)
	{
		m_pwndShow->ShowWindow(SW_SHOW);
		m_pwndShow = NULL;
	}

	switch(itab)
	{
	case 0:
		m_view1.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_view2.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_view3.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_view4.ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
}

void CSimulatorDebug::dlistColumn(int rCount,rxSystem** sys, rxDevice** device)
{
	CString buff;

	switch(rCount)
	{
	case 1:
		m_view1.m_robotID = RobotID[0];
		m_view2.allCount = DeviceCount;

		m_view1.TcnSelected(FALSE,sys,device);
		m_view2.TcnSelected(TRUE,sys,device);
		for (int i = 0 ; i < subDeviceCount[0] ; i++)
		{
			m_view1.m_dlist.InsertItem(i,DeviceInfoData[i].DeviceName);
			buff.Format(_T("%d"),DeviceInfoData[i].DevieID);
			m_view1.m_dlist.SetItemText(i,1,buff);
		}
		m_view1.subCount = subDeviceCount[0];
		break;
	case 2:
		m_view1.m_robotID = RobotID[0];
		m_view2.m_robotID = RobotID[1];
		m_view2.preCount = subDeviceCount[0];

		m_view3.allCount = DeviceCount;

		m_view1.TcnSelected(FALSE,sys,device);
		m_view2.TcnSelected(FALSE,sys,device);
		m_view3.TcnSelected(TRUE,sys,device);

		for (int i = 0 ; i < subDeviceCount[0] ; i++)
		{
			m_view1.m_dlist.InsertItem(i,DeviceInfoData[i].DeviceName);
			buff.Format(_T("%d"),DeviceInfoData[i].DevieID);
			m_view1.m_dlist.SetItemText(i,1,buff);
		}
		m_view1.subCount = subDeviceCount[0];
		for (int i = subDeviceCount[0] ; i < subDeviceCount[0] + subDeviceCount[1] ; i++)
		{
			m_view2.m_dlist.InsertItem(i-subDeviceCount[0],DeviceInfoData[i].DeviceName);
			buff.Format(_T("%d"),DeviceInfoData[i].DevieID);
			m_view2.m_dlist.SetItemText(i-subDeviceCount[0],1,buff);
		}
		m_view2.subCount = subDeviceCount[1];

		break;
	case 3:
		m_view1.m_robotID = RobotID[0];
		m_view2.m_robotID = RobotID[1];
		m_view2.preCount = subDeviceCount[0];
		m_view3.m_robotID = RobotID[2];
		m_view3.preCount = subDeviceCount[0] + subDeviceCount[1];
		m_view4.allCount = DeviceCount;

		m_view1.TcnSelected(FALSE,sys,device);
		m_view2.TcnSelected(FALSE,sys,device);
		m_view3.TcnSelected(FALSE,sys,device);
		m_view4.TcnSelected(sys,device);

		for (int i = 0 ; i < subDeviceCount[0] ; i++)
		{
			m_view1.m_dlist.InsertItem(i,DeviceInfoData[i].DeviceName);
			buff.Format(_T("%d"),DeviceInfoData[i].DevieID);
			m_view1.m_dlist.SetItemText(i,1,buff);
		}
		m_view1.subCount = subDeviceCount[0];
		for (int i = subDeviceCount[0] ; i < subDeviceCount[0] + subDeviceCount[1] ; i++)
		{
			m_view2.m_dlist.InsertItem(i-subDeviceCount[0],DeviceInfoData[i].DeviceName);
			buff.Format(_T("%d"),DeviceInfoData[i].DevieID);
			m_view2.m_dlist.SetItemText(i-subDeviceCount[0],1,buff);
		}
		m_view2.subCount = subDeviceCount[1];
		for (int i = subDeviceCount[0] + subDeviceCount[1]; 
			i < subDeviceCount[0] + subDeviceCount[1] + subDeviceCount[2]; i++)
		{
			m_view3.m_dlist.InsertItem
				(i-subDeviceCount[0] - subDeviceCount[1],DeviceInfoData[i].DeviceName);
			buff.Format(_T("%d"),DeviceInfoData[i].DevieID);
			m_view3.m_dlist.SetItemText
				(i-subDeviceCount[0] - subDeviceCount[1],1,buff);
		}
		m_view3.subCount = subDeviceCount[2];

		break;
	default:
		break;
	}
}
bool CSimulatorDebug::viewCheckBoxCtl(void)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	bool checkState = false;
	int i,j,count;

	DWORD cur_pos = ::GetMessagePos(); 
	CPoint point(LOWORD(cur_pos), HIWORD(cur_pos)); 
	pMain->m_deviceTree.ScreenToClient(&point); 
	UINT nFlags = 0; 
	HTREEITEM hItem = pMain->m_deviceTree.HitTest(point, &nFlags);
	if(hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		checkState = (bool)pMain->m_deviceTree.GetCheck(hItem);
		if (pMain->m_deviceTree.GetItemText(hItem) == _T("RobotList"))
		{
			for (i = 0 ; i < RobotCount ; i++)
			{
				pMain->m_deviceTree.SetCheck(RobotNode[i],!checkState);
			}
			for (i = 0 ; i < DeviceCount ; i++)
			{
				pMain->m_deviceTree.SetCheck(deviceNode[i],!checkState);
				if (checkState)
				{
					DeviceViewList[i].DeviceID = 0;
					DeviceViewList[i].RobotID = 0;
					DeviceViewList[i].DeviceName = "";
					DeviceViewList[i].RobotName = "";
				}
				else
				{
					DeviceViewList[i].DeviceID = DeviceInfoData[i].DevieID;
					DeviceViewList[i].RobotID  = DeviceInfoData[i].RobotID;
					DeviceViewList[i].DeviceName = DeviceInfoData[i].DeviceName;
					if (i < subDeviceCount[0])	DeviceViewList[i].RobotName = RobotName[0];
					else if ((subDeviceCount[0] <= i)&&(i < subDeviceCount[0] + subDeviceCount[1]))	DeviceViewList[i].RobotName = RobotName[1];
					else	DeviceViewList[i].RobotName = RobotName[2];
				}
			}
			if (checkState)		ListCount = 0;
			else				ListCount = DeviceCount;
			
			switch (RobotCount)
			{
			case 1:
				m_view2.GetDeviceList(DeviceViewList, ListCount);
				break;
			case 2:
				m_view3.GetDeviceList(DeviceViewList, ListCount);
				break;
			case 3:
				m_view4.GetDeviceList(DeviceViewList, ListCount);
				break;
			default:
				break;
			}
			return	true;
		}

		else
		{
			count = 0;
			j = 0;
			for (i = 0 ; i < RobotCount ; i++)
			{
				count = count + subDeviceCount[i];
				if (i == 0)	j = 0;
				else	j = j + subDeviceCount[i-1];
				if (pMain->m_deviceTree.GetItemText(hItem) == RobotName[i])
				{
					for (; j < count ; j++)
					{
						pMain->m_deviceTree.SetCheck(deviceNode[j],!checkState);
						if (checkState)
						{
							for (int k = 0 ; k < ListCount ; k++)
							{
								if (DeviceViewList[k].DeviceID == DeviceInfoData[j].DevieID)
								{
									for (int l = k ; l < ListCount ; l++)
									{
										DeviceViewList[l].DeviceID = DeviceViewList[l+1].DeviceID;
										DeviceViewList[l].RobotID  = DeviceViewList[l+1].RobotID;
										DeviceViewList[l].DeviceName = DeviceViewList[l+1].DeviceName;
										DeviceViewList[l].RobotName = DeviceViewList[l+1].RobotName;
									}
									DeviceViewList[ListCount-1].DeviceID = 0;
									DeviceViewList[ListCount-1].RobotID  = 0;
									DeviceViewList[ListCount-1].DeviceName = "";
									DeviceViewList[ListCount-1].RobotName = "";
									ListCount--;
									break;
								}
							}
						}
						else
						{
							bool dataflag = false;
							for (int k = 0 ; k < ListCount ; k++)
							{
								if(DeviceViewList[k].DeviceID == DeviceInfoData[j].DevieID)
								{
									dataflag = true;
									break;
								}
							}
							if (dataflag == false)
							{
								DeviceViewList[ListCount].DeviceID = DeviceInfoData[j].DevieID;
								DeviceViewList[ListCount].RobotID  = DeviceInfoData[j].RobotID;
								DeviceViewList[ListCount].DeviceName = DeviceInfoData[j].DeviceName;
								DeviceViewList[ListCount].RobotName = RobotName[i];
								ListCount++;
							}
						}
					}
					switch (RobotCount)
					{
					case 1:
						m_view2.GetDeviceList(DeviceViewList, ListCount);
						break;
					case 2:
						m_view3.GetDeviceList(DeviceViewList, ListCount);
						break;
					case 3:
						m_view4.GetDeviceList(DeviceViewList, ListCount);
						break;
					default:
						break;
					}
					return	true;
				}
			}
		}

		HTREEITEM	pItem = pMain->m_deviceTree.GetParentItem(hItem);
		CString pName = pMain->m_deviceTree.GetItemText(pItem);
		count = 0;
		for (i = 0 ; i < RobotCount ; i++)
		{
			if (i == 0 )	count  = 0;
			else	count = count + subDeviceCount[i-1];
			
			if (pName == RobotName[i])
			{
				for (j = 0 ; j < subDeviceCount[i] ; j++)
				{
					if (pMain->m_deviceTree.GetItemText(hItem) == DeviceInfoData[count].DeviceName)
					{
						if (checkState)
						{
							for (int k = 0 ; k < ListCount ; k++)
							{
								if (DeviceViewList[k].DeviceID == DeviceInfoData[count].DevieID)
								{
									for (int l = k ; l < ListCount ; l++)
									{
										DeviceViewList[l].DeviceID		= DeviceViewList[l+1].DeviceID;
										DeviceViewList[l].RobotID		= DeviceViewList[l+1].RobotID;
										DeviceViewList[l].DeviceName	= DeviceViewList[l+1].DeviceName;
										DeviceViewList[l].RobotName		= DeviceViewList[l+1].RobotName;
									}
									DeviceViewList[ListCount-1].DeviceID = 0;
									DeviceViewList[ListCount-1].RobotID  = 0;
									DeviceViewList[ListCount-1].DeviceName = "";
									DeviceViewList[ListCount-1].RobotName = "";
									ListCount--;
									break;
								}
							}
						}
						else
						{
							DeviceViewList[ListCount].DeviceID = DeviceInfoData[count].DevieID;
							DeviceViewList[ListCount].RobotID  = DeviceInfoData[count].RobotID;
							DeviceViewList[ListCount].DeviceName = DeviceInfoData[count].DeviceName;
							DeviceViewList[ListCount].RobotName = RobotName[i];
							ListCount++;
						}
						switch (RobotCount)
						{
						case 1:
							m_view2.GetDeviceList(DeviceViewList, ListCount);
							break;
						case 2:
							m_view3.GetDeviceList(DeviceViewList, ListCount);
							break;
						case 3:
							m_view4.GetDeviceList(DeviceViewList, ListCount);
							break;
						default:
							break;
						}
						return	true;
					}
					count++;
				}
			}
		}

	}
	return	false;
}

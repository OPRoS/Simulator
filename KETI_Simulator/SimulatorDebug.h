#pragma once

#include "resource.h"
#include "View1Dlg.h"
#include "View2Dlg.h"
#include "View3Dlg.h"
#include "View4Dlg.h"
#include "dataType.h"

class CKETI_SimulatorDlg;


class CSimulatorDebug
{
public:
	CSimulatorDebug(void);
	~CSimulatorDebug(void);

	CView1Dlg	m_view1;
	CView2Dlg	m_view2;
	CView3Dlg	m_view3;
	CView4Dlg	m_view4;

	HTREEITEM	deviceNode[256];
	HTREEITEM	RobotNode[3];

	void registerSimulator(Simulator* simulator);
	void simDebugInit(rxSystem** sys, rxDevice** device);
	void getTreeItem(rxSystem** sys, rxDevice** device);
	void SetTabgen(int viewNumber);
	void TcnSelected(int itab);
	void TcnSelecting(int itab);
	void CloseDialog(void);
	void dlistColumn(int rCount, rxSystem** sys, rxDevice** device);
	afx_msg	void	OnTimer(UINT_PTR	nIDEvent);

	int			RobotID[3];
	int			RobotCount;
	CString		RobotName[3];
	int			ListCount;
	
	struct Ddata{
		unsigned int		DevieID;
		int		RobotID;
		CString	DeviceName;
	};

	Vdata		DeviceViewList[256];
	Ddata		DeviceInfoData[256];
	int			DeviceCount;
private:
	Simulator* _simulator;
	CWnd* m_pwndShow;

public:
	virtual bool viewCheckBoxCtl(void);
};

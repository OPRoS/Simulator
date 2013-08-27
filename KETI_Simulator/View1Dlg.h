#pragma once
#include "afxcmn.h"

// CView1Dlg ��ȭ �����Դϴ�.

class Simulator;
//class RobotController;
//class SimulatorData;
//class SimulationController;
class CView1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CView1Dlg)

public:
	CView1Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CView1Dlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIEW1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	//RobotController*		m_RobotController;
	Simulator*				_simulator;
	rxDevice*				rdevice[256];
	CListCtrl				m_dlist;

	void TcnSelected(BOOL sel,rxSystem** sys, rxDevice** device);
	int	m_robotID;
	int All_RobotID[3];

	struct Ddata{
		int		DeviceID;
		int		RobotID;
		CString	DeviceName;
	};

	struct ConditionData{
		int ListNumber;
		int	Condition;
		double	Value;
	};

	Ddata	DeviceInfo[256];
	ConditionData	Cdata[256];
	int	CdataCount;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool	Allview;
	int subCount;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};

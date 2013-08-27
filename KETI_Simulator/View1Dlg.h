#pragma once
#include "afxcmn.h"

// CView1Dlg 대화 상자입니다.

class Simulator;
//class RobotController;
//class SimulatorData;
//class SimulationController;
class CView1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CView1Dlg)

public:
	CView1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CView1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

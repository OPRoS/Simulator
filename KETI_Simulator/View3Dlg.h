#pragma once
#include "afxcmn.h"
#include "dataType.h"

// CView3Dlg 대화 상자입니다.

class Simulator;
//class RobotController;

class CView3Dlg : public CDialog
{
	DECLARE_DYNAMIC(CView3Dlg)

public:
	CView3Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CView3Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//RobotController*		m_RobotController;
	Simulator*				_simulator;
	rxDevice*				rdevice[256];
	CListCtrl m_dlist;
	void TcnSelected(BOOL sel, rxSystem** sys, rxDevice** device);
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
	bool	Allview;
	int subCount;
	int allCount;
	int preCount;

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GetDeviceList(struct Vdata* data, int listCount);
	Vdata* m_data;
	int DeviceCount;
	afx_msg void OnNMDblclkDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};

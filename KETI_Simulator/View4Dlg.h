#pragma once
#include "afxcmn.h"
#include "dataType.h"




// CView4Dlg 대화 상자입니다.

class Simulator;
//class RobotController;

class CView4Dlg : public CDialog
{
	DECLARE_DYNAMIC(CView4Dlg)

public:
	CView4Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CView4Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	//RobotController*		m_RobotController;
	Simulator*				_simulator;
	rxDevice*				rdevice[256];

	CListCtrl m_dlist;
	void TcnSelected(rxSystem** sys, rxDevice** device);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GetDeviceList(struct Vdata* data, int listCount);

	int DeviceCount;
	int allCount;
	Vdata* m_data;

	struct ConditionData{
		int ListNumber;
		int	Condition;
		double	Value;
	};
	ConditionData	Cdata[256];
	int	CdataCount;


	afx_msg void OnNMDblclkDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};

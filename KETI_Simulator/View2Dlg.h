#pragma once
#include "afxcmn.h"
#include "dataType.h"

class Simulator;
//class RobotController;

// CView2Dlg ��ȭ �����Դϴ�.

class CView2Dlg : public CDialog
{
	DECLARE_DYNAMIC(CView2Dlg)

public:
	CView2Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CView2Dlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIEW2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	//RobotController*		m_RobotController;
	Simulator*				_simulator;
	rxDevice*				rdevice[256];
	CListCtrl m_dlist;

	void TcnSelected(BOOL sel, rxSystem** sys, rxDevice** device);
	int m_robotID;
	int All_RobotID[3];

	struct	Ddata{
		int		DeviceID;
		int		RobotID;
		CString	DeviceName;
	};
	
	struct ConditionData{
		int ListNumber;
		int	Condition;
		double	Value;
	};

	Ddata		DeviceInfo[256];
	ConditionData	Cdata[256];
	int	CdataCount;
	bool		Allview;
	int			subCount;
	int			allCount;
	int			preCount;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	void GetDeviceList(struct Vdata* data, int listCount);
	Vdata* m_data;
	int DeviceCount;
	afx_msg void OnNMDblclkDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDevicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};

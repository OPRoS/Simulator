#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CManipulatorDlg 대화 상자입니다.

class CManipulatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CManipulatorDlg)

public:
	CManipulatorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManipulatorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANIPULATOR };

	void registerSimulator(Simulator* simulator);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	 Simulator* _simulator;	 
public:
	CSliderCtrl SLink1;
	CSliderCtrl SLink2;
	CSliderCtrl SLink3;
	CSliderCtrl SLink4;
	CSliderCtrl SLink5;
	CSliderCtrl SLink6;
	CComboBox CLink1;
	CComboBox CLink2;
	CComboBox CLink3;
	CComboBox CLink4;
	CComboBox CLink5;
	CComboBox CLink6;
	CComboBox RobotName;
	double ELink1;
	double ELink2;
	double ELink3;
	double ELink4;
	double ELink5;
	double ELink6;
	int Link[6];
	int sliderNumber[6];

	int DCount;
	int	angleFlag;
	int runFlag;

	RobotInfo	rInfo;
	rxSystem*	rSys[3];
	rxDevice*	rDevice[256];

	virtual BOOL OnInitDialog();
	void SysDataInit(rxSystem** sys, rxDevice** device, RobotInfo rData);
	afx_msg void OnCbnCloseupCombo8();
	afx_msg void OnBnClickedScontrol();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedDeg();
	afx_msg void OnBnClickedRad();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNcDestroy();
};

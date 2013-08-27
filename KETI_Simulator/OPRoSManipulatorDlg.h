#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COPRoSManipulatorDlg ��ȭ �����Դϴ�.

class COPRoSManipulatorDlg : public CDialog
{
	DECLARE_DYNAMIC(COPRoSManipulatorDlg)

public:
	COPRoSManipulatorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COPRoSManipulatorDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MANIPULATOR_OPRoS };
	void registerSimulator(Simulator* simulator);
	Simulator* _simulator;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl SLink1;
	CSliderCtrl SLink2;
	CSliderCtrl SLink3;
	CSliderCtrl SLink4;
	CSliderCtrl SLink5;
	CSliderCtrl SLink6;
	CComboBox ComponentList;
	virtual BOOL OnInitDialog();
	BOOL InitSharedMemory(CString appname);
	HANDLE	m_hMap;
	LPBYTE	m_pSharedMemory;
	CString	appname;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	float ELink1;
	float ELink2;
	float ELink3;
	float ELink4;
	float ELink5;
	float ELink6;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int angleFlag;
	int runFlag;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
};

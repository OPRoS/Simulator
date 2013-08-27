#pragma once
#include "afxwin.h"


// CInputCondition 대화 상자입니다.

class CInputCondition : public CDialog
{
	DECLARE_DYNAMIC(CInputCondition)

public:
	CInputCondition(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputCondition();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONDITION_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double	GetValue()	const	{	return	Value;	}
	int		GetCondition()	const	{	return	count;	}
	double	Value;
	int		count;
	afx_msg void OnBnClickedOk();
	CComboBox ConditionList;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnCloseupCombo1();
};

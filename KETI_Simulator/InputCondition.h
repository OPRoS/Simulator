#pragma once
#include "afxwin.h"


// CInputCondition ��ȭ �����Դϴ�.

class CInputCondition : public CDialog
{
	DECLARE_DYNAMIC(CInputCondition)

public:
	CInputCondition(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputCondition();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONDITION_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

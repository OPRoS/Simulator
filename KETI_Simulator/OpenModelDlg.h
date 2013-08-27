#pragma once
#include "afxwin.h"
#include "resource.h"
#include "rMath/rMath.h"
using namespace rMath;

class COpenModelDlg :
	public CDialog
{
public:
	COpenModelDlg(CWnd* pParent = NULL);
	~COpenModelDlg(void);

	// Dialog Data
	enum { IDD = IDD_OPEN_MODEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	

public:
	afx_msg void OnBnClickedOpenFileDlg();
	CString GetModelName() const { return m_modelname; }
	CString GetFileName() const { return m_filename; }
	CString MPath;
	HTransform GetHTransform() const;
private:
	CString m_modelname;
	CString m_filename;
public:
	afx_msg void OnBnClickedOk();
private:
	float m_posX;
	float m_posY;
	float m_posZ;

	float m_rotX;
	float m_rotY;
	float m_rotZ;
};

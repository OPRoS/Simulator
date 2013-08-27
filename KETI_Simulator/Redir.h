#if !defined(AFX_REDIR_H__4FB57DC3_29A3_11D5_BB60_006097553C52__INCLUDED_)
#define AFX_REDIR_H__4FB57DC3_29A3_11D5_BB60_006097553C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRedirector : public CObject  
{
public:
	CEdit* m_pWnd;
	CRedirector();
	virtual ~CRedirector();
	
private:
	HANDLE m_hThread;		// thread to receive the output of the child process
	HANDLE m_hEvtStop;		// event to notify the redir thread to exit
	DWORD m_dwThreadId;		// id of the redir thread
	DWORD m_dwWaitTime;		// wait time to check the status of the child proces


protected:
	HANDLE m_hStdinWrite;	// write end of child's stdin pipe
	HANDLE m_hStdoutRead;	// read end of child's stdout pipe
	HANDLE m_hChildProcess;

	BOOL LaunchChild(HANDLE hStdOut, HANDLE hStdIn, HANDLE hStdErr);
	void DestroyHandle(HANDLE& rhObject);

	static DWORD WINAPI OutputThread(LPVOID lpvThreadParam);

protected:
	 //overrides:
	virtual void WriteStdOut(LPCTSTR pszOutput);
	virtual void WriteStdError(LPCTSTR pszError);

public:
	int RedirectStdout();
	BOOL Open();
	virtual void Close();
	BOOL Printf(char strInput[]);
	void SetWaitTime(DWORD dwWaitTime) { m_dwWaitTime = dwWaitTime; }
	void KillProcess(CString strProcessName);
	bool GetProcessModule(DWORD dwPID,CString sProcessName);
};

//extern CRedirector *m_redir;

#endif // !defined(AFX_REDIR_H__4FB57DC3_29A3_11D5_BB60_006097553C52__INCLUDED_)

#include "StdAfx.h"
#include "Tlhelp32.h"
#include "Simulator.h"
#include "OPRoSEngineRedirection.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"
#include <io.h>

OPRoSEngineRedirection::OPRoSEngineRedirection(void)
{
	RunAppIndex = 0;
	EngineStateFlag = 0;
}

OPRoSEngineRedirection::~OPRoSEngineRedirection(void)
{
}

void OPRoSEngineRedirection::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}

void OPRoSEngineRedirection::LoadAppList()
{
	_finddatai64_t c_file;
	intptr_t hFile;
	CString Name,name;
	int AppItemIndex = 0;
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	CString Path = pMain->RepositoryPath;
	Path += "\\*.xml";
	pMain->AppList.DeleteAllItems();
	if ( (hFile = _findfirsti64((CStringA)Path, &c_file)) == -1L)
	{

	}
	else
	{
		do{
			Name = c_file.name;
			AfxExtractSubString(name, Name, 0, '.');
			pMain->AppList.InsertItem(AppItemIndex++,name);
		}while(_findnexti64(hFile, &c_file) == 0);
	}
	_findclose(hFile);
}

void OPRoSEngineRedirection::viewComponentList(CString sIndexValue)
{
	int ComponentIndex = 0;
	_finddatai64_t c_file;
	intptr_t hFile;
	CString Name,name;
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();

	CString path;
	path = pMain->RepositoryPath;
	path += "\\";
	path += sIndexValue;
	path += "\\*.xml";

	if ( (hFile = _findfirsti64((CStringA)path, &c_file)) == -1L)
	{
		CString mes;
		mes = "error";
		pMain->AppList.SetItemText(ComponentIndex,1,mes);
	}
	else
	{
		do{
			Name = c_file.name;
			AfxExtractSubString(name, Name, 0, '.'); 
			pMain->AppList.SetItemText(ComponentIndex++,1,name);
		}while(_findnexti64(hFile, &c_file) == 0);
	}
	_findclose(hFile);
}

BOOL OPRoSEngineRedirection::RunApp(int AppIdx)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();

	CString buf;
	CString AppName = pMain->AppList.GetItemText(AppIdx , 0);

	GetProcess(pMain->EngineName);
	//GetProcess(_T("ComponentEngine.exe"));
	for (int i = 0 ; i < RunAppIndex ; i++)
	{
		pMain->AppRunList.GetText(i,buf);
		if (AppName == buf)
		{
			return 0;
		}
	}

	pMain->EngineState.SetWindowTextW(TEXT("OPRoS Engine Using"));

	if (EngineStateFlag ==  0)
	{
		pMain->AppRunList.ResetContent();
		RunAppIndex = 0;
		EngineStart();
		EngineAppRun(AppName);		
	}
	else if (EngineStateFlag == 1)
	{
		EngineAppRun(AppName);
	}

	pMain->AppRunList.AddString(AppName);
	RunAppIndex++;
	
	return 1;
}

void OPRoSEngineRedirection::DelRunAppList(int idx)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	
	if(GetProcess(pMain->EngineName))
//	if(GetProcess(_T("ComponentEngine.exe")))
	{
		CString appname;
		pMain->AppRunList.GetText(idx,appname);
		EngineAppStop(appname);
		pMain->AppRunList.DeleteString(idx);
		RunAppIndex--;
		if (RunAppIndex == 0)
		{
			EngineExit();
			EngineStateFlag = 0;
			pMain->EngineState.SetWindowTextW(TEXT("OPRoS Engine Disable"));
		}
	}
	else
	{
		pMain->AppRunList.ResetContent();
		RunAppIndex = 0;
		EngineStateFlag = 0;
	}
}

void OPRoSEngineRedirection::EngineStart()
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	if(GetProcess(pMain->EngineName))
	//if(GetProcess(_T("ComponentEngine.exe")))
	{
		pMain->MessageBox(
				TEXT("OPRoS Engine 이미 동작 중입니다.."),
				TEXT("OPRoS Engine Error"),
				MB_OK | MB_TOPMOST |MB_ICONWARNING);
	}
	else
	{
		m_redir.m_pWnd = (CEdit*)(pMain->GetDlgItem(IDC_OPRoS_CONSOLE));
		m_redir.m_pWnd->SetWindowTextW(TEXT(""));
		m_redir.Close();
		m_redir.Open();
		//m_redir.Printf("c:\r\n");
		CString Epath;
		Epath = pMain->EnginePath + _T("\r\n");
		//Epath  = _T("cd ") + pMain->EnginePath + _T("\r\n");
		char D_ch[128];
		strcpy(D_ch,(CStringA)Epath);
		char driveChange[5];
		driveChange[0] = D_ch[0];
		driveChange[1] = ':';
		driveChange[2] = '\r';
		driveChange[3] = '\n';
		driveChange[4] = 0;
		m_redir.Printf(driveChange);
		m_redir.Printf(D_ch);
		//char Engine[] = "ComponentEngine.exe\r\n";
		//m_redir.Printf(Engine);
		char hitkey[] = "4\r\n";
		m_redir.Printf(hitkey);

		pMain->AppRunList.ResetContent();
		RunAppIndex = 0;
	}
}

void OPRoSEngineRedirection::EngineAppRun(CString appname)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	if(GetProcess(pMain->EngineName))
	//if(GetProcess(_T("ComponentEngine.exe")))
	{
		CString CMD;
		CMD = "appname ";
		CMD += appname;
		CMD += "\r\n";
		char CMD_B[255];
		size_t CharactersConverted = 0;
		wcstombs_s(&CharactersConverted, CMD_B, CMD.GetLength()+1, CMD, _TRUNCATE); 
		m_redir.Printf(CMD_B);
		char RUN[] = "app run\r\n";
		m_redir.Printf(RUN);
		EngineStateFlag = 1;
	}
	else
	{
		CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
		m_redir.m_pWnd = (CEdit*)(pMain->GetDlgItem(IDC_OPRoS_CONSOLE));
		m_redir.m_pWnd->SetWindowTextW(TEXT(""));
		m_redir.Close();
		m_redir.Open();
		//m_redir.Printf("c:\r\n");
		CString Epath;
		Epath  = _T("cd ") + pMain->EnginePath + _T("\r\n");
		char D_ch[128];
		strcpy(D_ch,(CStringA)Epath);
		char driveChange[5];
		driveChange[0] = D_ch[3];
		driveChange[1] = ':';
		driveChange[2] = '\r';
		driveChange[3] = '\n';
		driveChange[4] = 0;
		m_redir.Printf(driveChange);
		m_redir.Printf(D_ch);
		CString Ename;
		Ename = pMain->EngineBatchFile + _T("\r\n");
		char Engine[128];
		strcpy(Engine,(CStringA)Ename);
		//char Engine[] = "ComponentEngine.exe\r\n";
		m_redir.Printf(Engine);
		char hitkey[] = "4\r\n";
		m_redir.Printf(hitkey);
		pMain->AppRunList.ResetContent();
		RunAppIndex = 0;
		CString CMD;
		CMD = "appname ";
		CMD += appname;
		CMD += "\r\n";
		char CMD_B[255];
		size_t CharactersConverted = 0;
		wcstombs_s(&CharactersConverted, CMD_B, CMD.GetLength()+1, CMD, _TRUNCATE); 
		m_redir.Printf(CMD_B);
		char RUN[] = "app run\r\n";
		m_redir.Printf(RUN);
		EngineStateFlag = 1;
	}
}

void OPRoSEngineRedirection::EngineAppStop(CString appname)
{
	CString CMD;
	CMD = "appname ";
	CMD += appname;
	CMD += "\r\n";
	char CMD_B[255];
	size_t CharactersConverted = 0;
	wcstombs_s(&CharactersConverted, CMD_B, CMD.GetLength()+1, CMD, _TRUNCATE); 
	m_redir.Printf(CMD_B);
	char RUN[] = "app stop\r\n";
	m_redir.Printf(RUN);
	EngineStateFlag = 0;
}

void OPRoSEngineRedirection::EngineExit()
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();

	char RUN[] = "app stop\r\n";
	m_redir.Printf(RUN);
	char EXIT[] = "exit\r\n";
	m_redir.Printf(EXIT);
	m_redir.Printf(EXIT);
	pMain->AppRunList.ResetContent();
	RunAppIndex = 0;
	EngineStateFlag = 0;
}

bool OPRoSEngineRedirection::GetProcess(CString processName)
{
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();
	HANDLE			hProcessSnap	=	NULL;
	BOOL			bRet			=	FALSE;
	PROCESSENTRY32	pe32			=	{0};
	hProcessSnap					=	CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if (hProcessSnap == (HANDLE)-1 )
		return false;

	CStringA ProcessName;
	ProcessName = processName;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32))
	{
		do{
			CStringA str;
			str = pe32.szExeFile;
			if (strcmp(str,ProcessName) == 0)
			{
				CloseHandle(hProcessSnap);
				EngineStateFlag = 1;
				//pMain->SetPath(false);
				return true;
			}
		}
		while(Process32Next(hProcessSnap,&pe32));
	}
	CloseHandle(hProcessSnap);
	EngineStateFlag = 0;
	//pMain->SetPath(true);
	return false;
}



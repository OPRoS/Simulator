#include "StdAfx.h"
#include "RobotControlServer.h"

RobotControlServer::RobotControlServer(void)
{
	if (!AfxSocketInit())
	{
		//AfxMessageBox("AfxSocketInit()의 초기화를 실패 하였습니다.");
		m_bActivate = false ;
	}
	else
	{
		m_bActivate = true;
	}
	m_bStarted = false ;
}

RobotControlServer::~RobotControlServer(void)
{
	StopServer();
}

bool RobotControlServer::StartServer(int dPortNum)
{
	if( (m_bActivate) && (!m_bStarted) )
	{
		m_dSocketCount = 0;
		m_RobotServer.Create(dPortNum);
		m_RobotServer.Listen();
		m_bStarted = true ;
		return true ;
	}	
	else
	{
		return false ;
	}
}

bool RobotControlServer::StopServer( void )
{
	int i  ;
	if( m_bStarted )
	{
		for(i = 0 ; i < 100 ; i++ )
		{
			if( m_dataSocket[i].m_dClientId != -1 )
			{
				m_dataSocket[i].m_dClientId = -1 ;
				m_dataSocket[i].Close() ;
				m_dSocketCount-- ;
			}

			if( m_dSocketCount == 0 ) break ;
		}

		m_RobotServer.Close() ;
		m_bStarted = false ;

		return true ;
	}
	else
	{
		return false ;
	}
}

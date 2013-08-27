#pragma once

#include "RobotServer.h"



class RobotControlServer
{
public:
	RobotControlServer(void);
	~RobotControlServer(void);

	RobotServer	m_RobotServer;
	RobotServer	m_dataSocket[100];

	int     m_dSocketCount;						
	bool	m_bActivate ;
	bool	m_bStarted ;

	bool StartServer( int portNum ) ;
	bool StopServer( void ) ;

};

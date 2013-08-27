#pragma once
#include <afxsock.h>


// RobotServer 명령 대상입니다.
class simualtor;

class RobotServer : public CSocket
{
public:
	int m_dSvrId ;
	int m_dClientId ;

	char m_cPacketBuf[65536] ;
	unsigned int m_udCurPos ;
	char m_cMadePacket[65536] ;
	char *data;
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	RobotServer();
	virtual ~RobotServer();
};



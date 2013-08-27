// RobotServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Simulator.h"
#include "RobotServer.h"
#include "RobotControlServer.h"
#include "RobotControlData.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"

#include "stringUtils.h"
#include "dataType.h"

extern RobotControlServer	g_RobotControlServer;
extern RobotControlData		*g_RobotControlData;

// RobotServer

RobotServer::RobotServer()
{
	m_dClientId = -1 ;		// 유효하지 않은 ID값.
	m_dSvrId = -1 ;
	m_udCurPos = 0;
}

RobotServer::~RobotServer()
{
	if( m_dClientId != -1 )
	{
		this->Close() ;
		m_dClientId = -1 ;
		m_dSvrId = -1 ;
	}
	delete []data;
}


// RobotServer 멤버 함수

void RobotServer::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	int i = 0 ;
	

	if( g_RobotControlServer.m_dSocketCount < 100 )
	{	
		for( i = 0 ; i < 100 ; i++ )
		{
			if( g_RobotControlServer.m_dataSocket[i].m_dClientId == -1)
			{
				g_RobotControlServer.m_RobotServer.Accept(
					g_RobotControlServer.m_dataSocket[i]);
				g_RobotControlServer.m_dataSocket[i].m_dSvrId = i;
				g_RobotControlServer.m_dataSocket[i].m_dClientId = 0;
				g_RobotControlServer.m_dSocketCount++;
				g_RobotControlServer.m_RobotServer.Listen();
				return ;	
			}
		}
	}
	else
	{
	}

	CSocket::OnAccept(nErrorCode);
}

void RobotServer::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	this->Close() ;
	m_dClientId = -1 ;		// 유효하지 않은 ID값.
	g_RobotControlServer.m_dSocketCount--;

	CSocket::OnClose(nErrorCode);
}
//-------------------------------------------------------------------------
//	Request Message
//	0x01	:	Init Device
//	0x02	:	Read Device
//	0x03	:	Write Device
//	0x06	:	Image Data
//-------------------------------------------------------------------------

void RobotServer::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();

	char msgchar[10000];
	memset(&msgchar,0,10000);
	Receive(&msgchar, sizeof(msgchar));

	if (msgchar[0] == 0x01)
	{
		initDevice		recvInfo;
		findDeviceID	sendID;

		memset(&recvInfo,0,sizeof(recvInfo));
		memcpy(&recvInfo,msgchar, sizeof(msgchar));
		
		sendID.cnt = recvInfo.deviceCnt;
		sendID.command = 0x01;

		for (int i = 0 ; i < g_RobotControlData->RobotCnt ; i++)
		{
			char Name[30];
			string name;
			GetStringFromStringType(name, g_RobotControlData->m_sys[i]->name());
			strcpy_s(Name,name.c_str());
			if(strcmp(recvInfo.RobotName,Name) == 0)
			{
				for (int j = 0 ; j < recvInfo.deviceCnt ; j++)
				{
					int	dstart	= g_RobotControlData->DeviceCnt[i];
					int	dend	= g_RobotControlData->DeviceCnt[i+1];
					for (int k = dstart ; k < dend ; k++)
					{
						memset(&Name,0,30);
						GetStringFromStringType(name, g_RobotControlData->m_device[k]->name());
						strcpy_s(Name,name.c_str());
						if (strcmp(recvInfo.DeviceName[j],Name) == 0)
						{
							sendID.deviceID[j] = k;
							break;
						}
					}
				}
				//memset(&sendID,0,sizeof(sendID));
				char *dIDmsg = new char [sizeof(sendID)];
				memcpy(dIDmsg,&sendID,sizeof(sendID));
				Send(dIDmsg,sizeof(sendID),0);
				delete [] dIDmsg;				
				CSocket::OnReceive(nErrorCode);
				return;
			}
		}
	}
	else if (msgchar[0] == 0x02)
	{
		Reqmsg msg;
		DeviceValue smsg;
		memcpy(&msg,msgchar,sizeof(msgchar));
		memset(&smsg,0,sizeof(smsg));
		smsg.command = 0x02;
		smsg.ID = msg.ID;
		smsg.cnt = g_RobotControlData->m_device[msg.ID]->
			readDeviceValue((void*)smsg.value,sizeof(float)*1000)/4;
		char *csmsg = new char [sizeof(smsg)];
		memcpy(csmsg,&smsg,sizeof(smsg));
		Send(csmsg,sizeof(smsg),0);
		delete [] csmsg;
		CSocket::OnReceive(nErrorCode);
		return;
	}

	else if (msgchar[0] == 0x03)
	{
		DeviceValue	wdevice;		
		memcpy (&wdevice,msgchar, sizeof(msgchar));

		g_RobotControlData->m_device[wdevice.ID]->writeDeviceValue((void*)wdevice.value,sizeof(float)*wdevice.cnt);
		return;
		
	}
	else if (msgchar[0] == 0x04)
	{
		double po[3];
		po[0] = 0.2;
		po[1] = 0.3;
		po[2] = 0.12;

		char *rpmsg = new char [24];
		memcpy (rpmsg,&po[0],sizeof(po));
		while(1)
		{
			Send(rpmsg,sizeof(po),0);
			//Sleep(100);
		}
	}

	else if (msgchar[0] == 0x05)
	{
		Reqmsg msg;
		DeviceValue smsg;
		memcpy(&msg,msgchar,sizeof(msgchar));
		memset(&smsg,0,sizeof(smsg));
		smsg.command = 0x05;
		smsg.ID = 0;
		smsg.cnt = 1;
		smsg.value[0] = pMain->m_simulator->getTime();
		char *csmsg = new char [sizeof(smsg)];
		memcpy(csmsg,&smsg,sizeof(smsg));
		Send(csmsg,sizeof(smsg),0);
		delete [] csmsg;
		CSocket::OnReceive(nErrorCode);
		return;
	}

	else if(msgchar[0] == 0x06)
	{
		Reqmsg msg;
		ImageData	smsg;
		memcpy(&msg,msgchar,sizeof(msgchar));
		memset(&smsg,0,sizeof(smsg));
		smsg.command = 0x06;
		smsg.ID = msg.ID;
		smsg.cnt = g_RobotControlData->m_device[msg.ID]->
			readDeviceValue((void*)smsg.Image,230400);
		char *csmsg = new char [sizeof(smsg)];
		memcpy(csmsg,&smsg,sizeof(smsg));
		Send(csmsg,sizeof(smsg),0);
		delete [] csmsg;
		CSocket::OnReceive(nErrorCode);
		return;
	}


	CSocket::OnReceive(nErrorCode);
}

int RobotServer::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CSocket::Receive(lpBuf, nBufLen, nFlags);
}

int RobotServer::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CSocket::Send(lpBuf, nBufLen, nFlags);
}

#include "stdafx.h"
#include "KETI_Simulator.h"
#include "rDevice/rDeviceType.h"
#include "protocol.h"
#include "Simulator.h"
#include "RobotController.h"
//#include	"rxSDKCommon/rxUtilities.h"	// 10/02/19 수정
#include "stringUtils.h"

#define binth_N

RobotController::RobotController() : _simulator(NULL)
{
	create(ROBOT_CONTROL_PORT, kaiON_ACCEPT | kaiON_MESSAGE | kaiON_CLOSE);
	run(true, 50);
}

RobotController::~RobotController()
{

}

void RobotController::onMessage(int id, kaiMsg& msg)
{
	if (!_simulator)
		return;

	kaiSocket* sock = findClient(id);

#ifdef binth_x
	TCHAR* message = TEXT("on Message - find Client\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif

	msg.begin();
	switch(msg.id())
	{
	case KP_CMD_MOTOR_VALUE:
		{

#ifdef binth	
			TCHAR* message = TEXT("(1)on Message - KP_CMD_MOTOR_VALUE\n");
			SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif

			rID deviceid;
			int cnt;
			msg >> deviceid >> cnt;

			for (int i = 0; i < 2; i++)
				msg >> buffer[i];

#ifdef binth	
			TCHAR message2[80];
			swprintf(message2, 80, TEXT("Device ID, CNT : [%d], [%d], [%f], [%f]\n"), deviceid, cnt, buffer[0], buffer[1]);
			SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message2, NULL);

#endif

			rxDevice* device = findDevice(deviceid);
			if (device)
				//device->writeDeviceValuefv(buffer, cnt);	// 10/02/19 수정
				device->writeDeviceValue((void*)buffer, sizeof(float)*2);
		}
		break;

	case KP_REQ_SENSOR_VALUE:
		{

#ifdef binth	
	TCHAR* message = TEXT("(2)on Message - KP_REQ_SENSOR_VALUE\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif
			rID deviceid;
			msg >> deviceid;
			kaiMsg answer;
			answer.setBuffer(msgbuffer, 4096);
			answer.begin();
			short cnt;
			rxDevice* device = findDevice(deviceid);
			if (!device)
			{
				cnt = 0;
				answer << cnt;
		
			}
			else
			{
				cnt = 1;
				//int type = device->typeInDetail();	//	09/08/19 수정
				//int dataCnt = device->readDeviceValuefv(buffer, 1000)/4;	// 10/02/19 수정
				int dataCnt = device->readDeviceValue((void*)buffer, sizeof(float)*1000)/4;
				//answer << deviceid << type << dataCnt;	//	09/08/19 수정
				answer << deviceid << dataCnt;
				for (int i = 0; i < dataCnt; i++){
					answer << buffer[i];
#ifdef binth_x
					TCHAR message[40];
					wsprintf(message, TEXT("Value : [%d]\n"), buffer[i] );
					SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif
#ifdef binth	
					TCHAR message2[80];
					swprintf(message2, 80, TEXT("Value : [%f]\n"), buffer[i]);
					SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message2, NULL);

#endif
				}

			}
			answer.id(KP_ACK_SENSOR_VALUE);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_DEVICE_LIST:
		{

#ifdef binth	
			TCHAR* message = TEXT("(3)on Message - KP_REQ_DEVICE_LIST\n");
			SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif

			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);

#ifdef binth	
			TCHAR message2[40];
			wsprintf(message2, TEXT("Robot ID : [%d]\n"), id);
			SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message2, NULL);
#endif

			short cnt;
			kaiMsg answer;
			answer.setBuffer(msgbuffer, 4096);
			answer.begin();
			if(!sys)
			{	
				cnt = 0;
				answer << cnt;				
			}
			else
			{
				DeviceList& devices = sys->getDevices();
				cnt = devices.size();
				answer << cnt;


#ifdef binth	
				TCHAR message[40];
				wsprintf(message, TEXT("Size : [%d]\n"), cnt);
				SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif


				for (DeviceList::iterator iter = devices.begin(); iter != devices.end(); ++iter)
				{
					rxDevice* device = *iter;
					string name;
					GetStringFromStringType(name, device->name());

					/*	09/08/19 수정
					int dataCnt = 0;

					if (device->type() & RD_CATEGORY_SENSOR)
					{
						dataCnt = device->readDeviceValuefv(buffer, 1000)/4;
					}

					if (device->type() & RD_CATEGORY_ACTUATOR)
					{
						memset(buffer, 0, 1000);	// added 090515
						dataCnt = device->writeDeviceValuefv(buffer, 1000)/4;
					}	
					answer << device->id() << name << device->typeInDetail() << dataCnt;
					*/

					answer << device->id() << name;

#ifdef binth	
					TCHAR message[40];
					wsprintf(message, TEXT("Value : ID[%d] NAME[%ls]\n"), device->id(), name );
					SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif

				
					/*	장이 버젼 - 데모를 위해 주석처리

					int readCnt = 0;
					int writeCnt = 0;
	
					//	device types are sensor(read), actuator(write) and both(read/write).

					if (device->type() & RD_CATEGORY_SENSOR)
					{
						readCnt = device->readDeviceValuefv(buffer, 1000)/4;
					}

					if (device->type() & RD_CATEGORY_ACTUATOR)
					{
						ZeroMemory(buffer, 1000);	// clear buffer
						writeCnt = device->writeDeviceValuefv(buffer, 1000)/4;						
					}					
			
					answer << device->id() << name << device->typeInDetail() << readCnt << writeCnt;	
					*/
				}
			}
			answer.id(KP_ACK_DEVICE_LIST);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_CMD_RESET_DEVICE:
		{

		}
		break;
	case KP_MOVE_DOWN:
		{
			_simulator->moveDown();
			kaiMsg answer;
			answer.allocateMemory(20);
			answer.begin();
			answer.id(KP_ACK_MOVE_DOWN);
			answer.end();
			sock->send(answer);
		}
		break;
	case KP_MOVE_UP:
		{
			_simulator->moveUp();
			kaiMsg answer;
			answer.allocateMemory(20);
			answer.begin();
			answer.id(KP_ACK_MOVE_UP);
			answer.end();
			sock->send(answer);
		}
		break;

	}


}

rxDevice* RobotController::findDevice(rID id)
{
	if (!_simulator)
		return NULL;

	SystemList& systems = _simulator->getSystems();
	for (SystemList::iterator iter = systems.begin(); iter != systems.end(); ++iter)
	{
		rxSystem* system = (*iter);
		rxDevice* device = system->findDevice(id);
		if (device)
			return device;
	}
	
	return NULL;
}

void RobotController::onClose(int id)
{
	TCHAR* message = TEXT("robot controller is disconeected\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);

}

/*	추가: 김영중
		로드된 모든 aml, eml 파일들의 (id, name) 정보를 보낸다.
*/
void RobotController::onAccept(int id)
{
	kaiSocket* sock = findClient(id);
	kaiMsg msg;
	msg.setBuffer(msgbuffer, 4096);
	msg.begin();

	SystemList& systems = _simulator->getSystems();
	short cnt = systems.size();
	msg << cnt;
	for (SystemList::iterator iter = systems.begin(); iter != systems.end(); ++iter)
	{
		rxSystem* sys = (*iter);
		string name;
		GetStringFromStringType(name, sys->name());
		msg << sys->id() << name;

#ifdef binth	
		TCHAR message[40];
		wsprintf(message, TEXT("R_ID : [%d]\n"), sys->id());
		SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
#endif

	}

	/* 장이 버젼 - 데모를 위해 주석처리

	//	send environment infoes
	EnvironmentList& envs= _simulator->getEnvironments();
	cnt = envs.size();
	msg << cnt;
	for (EnvironmentList::iterator iter = envs.begin(); iter != envs.end(); ++iter)
	{
		rxEnvironment * env = (*iter);
		string name;
		GetStringFromStringType(name, env->name());
		msg << env->id() << name;
	}
	*/
	
	msg.id(KP_NOTI_ROBOT_INFOES);
	msg.end();

	sock->send(msg);


	TCHAR* message = TEXT("Simulation controller is coneected\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
}

void RobotController::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}
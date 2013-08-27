#include "stdafx.h"
#include "KETI_Simulator.h"
#include "rDevice/rDeviceType.h"
#include "protocol.h"
#include "Simulator.h"
#include "SimulatorData.h"
//#include	"rxSDKCommon/rxUtilities.h"	// 10/02/19 수정
#include "stringUtils.h"

SimulatorData::SimulatorData() : _simulator(NULL)
{
	create(SIMULATOR_DATA_PORT, kaiON_ACCEPT | kaiON_MESSAGE | kaiON_CLOSE);
	run(true, 50);
}

SimulatorData::~SimulatorData()
{

}

void SimulatorData::onMessage(int id, kaiMsg& msg)
{
	if (!_simulator)
		return;

	kaiSocket* sock = findClient(id);
	msg.begin();
	switch(msg.id())
	{
	case KP_REQ_START_SIMULATION:
		{
			_simulator->activate();
			kaiMsg answer;
			answer.allocateMemory(10);
			answer.begin();
			answer.id(KP_ACK_START_SIMULATION);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_STOP_SIMULATION:
		{
			_simulator->deactivate();
			kaiMsg answer;
			answer.allocateMemory(10);
			answer.begin();
			answer.id(KP_ACK_STOP_SIMULATION);
			answer.end();
			sock->send(answer);
		}
		break;
	case KP_REQ_BODY_LIST:
		{
			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);
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
				BodyList& bodies = sys->bodies();
				cnt = bodies.size();
				answer << cnt;
				for (BodyList::iterator iter = bodies.begin(); iter != bodies.end(); ++iter)
				{
					rxBody* body = *iter;
					string name;
					GetStringFromStringType(name, body->name());

					answer << body->id() << name;
				}
			}
			answer.id(KP_ACK_BODY_LIST);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_BODY_DATA:
		{
			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);
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
				BodyList& bodies = sys->bodies();
				cnt = bodies.size();
				answer << cnt;
				answer << PHYSICS_WORLD->time();
				for (BodyList::iterator iter = bodies.begin(); iter != bodies.end(); ++iter)
				{
					rxBody* body = *iter;
					string name;
					GetStringFromStringType(name, body->name());

					answer << name;

					HTransform T = body->T();
					answer << T.R(0,0) << T.R(0,1) << T.R(0,2);
					answer << T.R(1,0) << T.R(1,1) << T.R(1,2);
					answer << T.R(2,0) << T.R(2,1) << T.R(2,2);
					answer << T.r[0] << T.r[1] << T.r[2];

					Twist V = body->V();
					answer << V.v(0) << V.v(1) << V.v(2);
					answer << V.w(0) << V.w(1) << V.w(2);
				}
			}	
			answer.id(KP_ACK_BODY_DATA);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_JOINT_LIST:
		{
			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);
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
				JointList& joints = sys->joints();
				cnt = joints.size();
				answer << cnt;
				for (JointList::iterator iter = joints.begin(); iter != joints.end(); ++iter)
				{
					rxJoint* joint = *iter;
					string name;
					GetStringFromStringType(name, joint->name());

					answer << joint->id() << name;
				}
			}
			answer.id(KP_ACK_JOINT_LIST);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_JOINT_DATA:
		{
			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);
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
				JointList& joints = sys->joints();
				cnt = joints.size();
				answer << cnt;
				answer << PHYSICS_WORLD->time();
				for (JointList::iterator iter = joints.begin(); iter != joints.end(); ++iter)
				{
					rxJoint* joint = *iter;
					string name;
					GetStringFromStringType(name, joint->name());

					answer << name;

					HTransform T = joint->T();
					answer << T.R(0,0) << T.R(0,1) << T.R(0,2);
					answer << T.R(1,0) << T.R(1,1) << T.R(1,2);
					answer << T.R(2,0) << T.R(2,1) << T.R(2,2);
					answer << T.r[0] << T.r[1] << T.r[2];

					Twist V = joint->Vadj();
					answer << V.v(0) << V.v(1) << V.v(2);
					answer << V.w(0) << V.w(1) << V.w(2);
				}
			}
			answer.id(KP_ACK_JOINT_DATA);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_DEVICE_LIST:
		{
			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);
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
				for (DeviceList::iterator iter = devices.begin(); iter != devices.end(); ++iter)
				{
					rxDevice* device = *iter;
					string name;
					GetStringFromStringType(name, device->name());

					answer << device->id() << name << device->userType() << 0;			
				}
			}
			answer.id(KP_ACK_DEVICE_LIST);
			answer.end();
			sock->send(answer);
		}
		break;

	case KP_REQ_DEVICE_DATA:
		{
			rID id;
			msg >> id;

			rxSystem* sys = _simulator->findSystem(id);
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
				answer << PHYSICS_WORLD->time();
				for (DeviceList::iterator iter = devices.begin(); iter != devices.end(); ++iter)
				{
					rxDevice* device = *iter;
					string name;
					GetStringFromStringType(name, device->name());

					answer << name;

					//rxDevice* device = findDevice(device->id());
					if (!device)
					{
						cnt = 0;
						answer << cnt;
				
					}
					else
					{
						int type = device->userType();
						//cnt = device->readDeviceValuefv(buffer, 1000)/4;	// 10/02/19 수정
						cnt = device->readDeviceValue((void*)buffer, sizeof(float)*1000)/4;
						answer << cnt;
						for (int i = 0; i < cnt; i++)
							answer << buffer[i];

					}
				}
			}
			answer.id(KP_ACK_DEVICE_DATA);
			answer.end();
			sock->send(answer);
		}
		break;
	}

}

rxDevice* SimulatorData::findDevice(rID id)
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

void SimulatorData::onClose(int id)
{
	TCHAR* message = TEXT("robot controller is disconeected\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);

}

/*	추가: 김영중
		로드된 모든 aml, eml 파일들의 (id, name) 정보를 보낸다.
*/
void SimulatorData::onAccept(int id)
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

void SimulatorData::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}
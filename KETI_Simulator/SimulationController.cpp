#include "stdafx.h"
#include "protocol.h"
#include "KETI_Simulator.h"
#include "Simulator.h"
#include "SimulationController.h"
//#include	"rxSDKCommon/rxUtilities.h"	// 10/02/19 ¼öÁ¤
#include "stringUtils.h"

SimulationController::SimulationController() : _simulator(NULL)
{
	create(SIMULATION_CONTROL_PORT, kaiON_ACCEPT | kaiON_MESSAGE | kaiON_CLOSE);
	run(true, 200);
}

SimulationController::~SimulationController()
{

}


void SimulationController::onMessage(int id, kaiMsg& msg)
{
	if (!_simulator)
		return;

	msg.begin();
	kaiSocket* sock = findClient(id);
	switch(msg.id())
	{	
	case KP_REQ_CONNECT:
		{
			kaiMsg answer;
			answer.allocateMemory(10);
			answer.begin();
			answer.id(KP_ANS_CONNECT);
			answer.end();
			sock->send(answer);
		}
		break;

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

	case KP_REQ_LOAD_MODEL:
		{
			string name;
			string filename;
			msg >> filename >> name;

			string_type name_unicode;
			string_type filename_unicode;

			GetStringTypeFromString(name_unicode, name);
			GetStringTypeFromString(filename_unicode, filename);
			
			kaiMsg answer;
			answer.allocateMemory(10);
			answer.begin();
			if (_simulator->loadModel(filename_unicode, name_unicode, HTransform()))
				answer << SUCCESS_LOAD_MODEL;
			else
				answer << FAIL_LOAD_MODEL;
			answer.id(KP_ACK_LOAD_MODEL);
			answer.end();

			sock->send(answer);
		}
		break;

	case KP_REQ_UNLOAD_MODEL:
		{
			rID id;
			msg >> id;

			kaiMsg answer;
			answer.allocateMemory(10);
			answer.begin();
			if (_simulator->unloadModel(id))
				answer << SUCCESS_UNLOAD_MODEL;
			else
				answer << FAIL_UNLOAD_MODEL;
			answer.id(KP_ACK_UNLOAD_MODEL);
			answer.end();
			sock->send(answer);
		}
		break;


	}

}

void SimulationController::onAccept(int id)
{
	TCHAR* message = TEXT("Simulation controller is coneected\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
}

void SimulationController::onClose(int id)
{
	TCHAR* message = TEXT("Simulation controller is disconeected\n");
	SendMessage(theApp.m_pMainWnd->m_hWnd, SEND_LOG_MESSGE, (WPARAM)message, NULL);
}

void SimulationController::registerSimulator(Simulator *simulator)
{
	_simulator = simulator;
}
#include "stdafx.h"
#include "rTerm/rTerm.h"
using namespace rTerm;
#include "Simulator.h"
//#include	"rxSDKCommon/rxUtilities.h"	// 10/02/19 수정
#include "stringUtils.h"

static	const	float	MOVE_DELTA	=	0.02f;


Simulator::Simulator() 
: _period(0.005f)
, _bRun(false)
, _bStarted(false)
{

	_movableBoxPtr	=	0;

	PHYSICS_WORLD->makeSimulationWorld(true);
	PHYSICS_WORLD->createPlane(0,0,1,0);
	PHYSICS_WORLD->setGravity(0,0,-GRAV_ACC);
	PHYSICS_WORLD->makeNetwork(0);

	startPlayer("-ip 127.0.0.1 -port 5150");

	//startProcess(
	//	"C:/Program Files/OPRoS/OPRoS_Simulator/Simulator/bin/rPlayer_OPRoS.exe",
	//	"-ip 127.0.0.1 -port 5150", true);
}

Simulator::~Simulator()
{
	// RoboticsLab 1.8
	//stopPlayer();
	stopProcess("rPlayer_OPRoS");
	DESTROY_PHYSICS_WORLD();
}


void	Simulator::clearDisplay()	{

	_movableBoxPtr	=	0;

	PHYSICS_WORLD->makeSimulationWorld(true);
	PHYSICS_WORLD->createPlane(0,0,1,0);
	PHYSICS_WORLD->setGravity(0,0,-GRAV_ACC);
	PHYSICS_WORLD->makeNetwork(0);

	startPlayer("-ip 127.0.0.1 -port 5150");
	// RoboticsLab 1.8
//	PHYSICS_WORLD->clearDisplay();
}

void Simulator::reset()
{
	PHYSICS_WORLD->reset();
	PHYSICS_WORLD->createPlane(0,0,1,0);
	PHYSICS_WORLD->setGravity(0,0,-GRAV_ACC);
	_bStarted = false;
}

void	Simulator::initialize()	{
	PHYSICS_WORLD->initialize();
	SetKeyboardHandler();
}

double Simulator::getTime()
{
	double gtime;

	gtime = PHYSICS_WORLD->time();

	return gtime;
}	

bool Simulator::loadModel(const string_type& filename, const string_type& modelname, const HTransform& T0)
{
	
	size_t pos = filename.find_last_of('.');
	string_type ext = filename.substr(pos+1, filename.size());
	
	bool bLoad = false;
	if (ext == _T("aml"))
	{
		if (PHYSICS_WORLD->createSystem(filename, modelname, T0))	{
			bLoad = true;
		}
	}
	else if (ext == _T("eml"))
	{
		if (PHYSICS_WORLD->createEnvironment(filename, modelname, T0))
			bLoad = true;
	}

	return bLoad;
}


rID	Simulator::loadSystem(const string_type& filename, const string_type& modelname, const HTransform& T0)
{
	rxSystem	*	pSys	=	PHYSICS_WORLD->createSystem(filename, modelname, T0);
	_ASSERTE(pSys != 0);

	return	pSys->id();
}

rID	Simulator::loadEnvironment(const string_type& filename, const string_type& modelname, const HTransform& T0)
{
	rxWorld	*	pWorld	=	rxWorld::getInstance();
	rxEnvironment	*	pEnv	=	PHYSICS_WORLD->createEnvironment(filename, modelname, T0);
	_ASSERTE(pEnv != 0);

	if	(pEnv)	{

		SystemList		sbodyList = pEnv->getSystems();
		SystemList::const_iterator itr = sbodyList.begin();

		//StaticBodyList	sbodyList	=	pEnv->getStaticBodies();
		//StaticBodyList::const_iterator	itr	=	sbodyList.begin();


		//string_type	body_name	=	(*itr)->name();
		string_type	box_name	=	_T("box_movable");

		_movableBoxPtr	=	0;

		for	(; itr != sbodyList.end(); ++itr)	{
			if	(*(*itr) == box_name)	{
				_movableBoxPtr	=	(DWORD) (*itr);
				break	;
			}
		}
	}

	return	pEnv->id();
}

void Simulator::setPeriod(rTime period)
{
	_period = period;
}

bool Simulator::update()
{
	if (_bRun)
	{
		PHYSICS_WORLD->update(_period);
		PHYSICS_WORLD->handleKeyEvent();

		return true;
	}
	return false;
}

SystemList& Simulator::getSystems()
{
	return PHYSICS_WORLD->getSystems();
}

EnvironmentList & Simulator::getEnvironments()
{
	return PHYSICS_WORLD->getEnvironments();
}

bool Simulator::unloadModel(rID id)
{
	//return PHYSICS_WORLD->destroySystem(id);

	stopProcess("rPlayer_OPRoS");
	DESTROY_PHYSICS_WORLD();

	return true;
}

bool Simulator::unloadEnv(rID id)
{
	return	PHYSICS_WORLD->destroyEnvironment(id);
}

void Simulator::activate()
{
	if (!_bStarted)
	{
		PHYSICS_WORLD->initialize();
		_bStarted = true;
	}
	_bRun = true;
}

void Simulator::deactivate()
{
	_bRun = false;
}

rxSystem* Simulator::findSystem(rID id)
{
	return PHYSICS_WORLD->findSystem(id);
}

void	Simulator::T(rID id, const HTransform & T0)	{
	rxSystem	*	pSys	=	PHYSICS_WORLD->findSystem(id);
	pSys->T(T0);
}

void OnKeyLeft(void* data)
{
	//HTransform T0(Rotation(), Displacement(0.f, 0.f, 0.2f));
	//m_simulator->T(itr->second.model_id, T0);
	//m_simulator->T(itr->second.model_id, _transform_org); // 초기 위치 재적용

	DWORD * pMovableBoxPtr = (DWORD*)data;

	rxStaticBody * pBox = (rxStaticBody*) *pMovableBoxPtr;
	// pos of model + delt of pos
	if (pBox != 0) {
		HTransform T0 = pBox->T();
		HTransform T1 = T0;

		T1.r[0] -= MOVE_DELTA;
		pBox->T(T1);
	}
}

void OnKeyRight(void* data)
{
	//HTransform T0(Rotation(), Displacement(0.f, 0.f, 0.2f));
	//m_simulator->T(itr->second.model_id, T0);
	//m_simulator->T(itr->second.model_id, _transform_org); // 초기 위치 재적용


	DWORD * pMovableBoxPtr = (DWORD*)data;

	rxStaticBody * pBox = (rxStaticBody*) *pMovableBoxPtr;

	// pos of model + delt of pos
	if (pBox != 0) {
		HTransform T0 = pBox->T();
		HTransform T1 = T0;

		T1.r[0] += MOVE_DELTA;
		pBox->T(T1);
	}
}


void OnKeyFront(void* data)
{
	//HTransform T0(Rotation(), Displacement(0.f, 0.f, 0.2f));
	//m_simulator->T(itr->second.model_id, T0);
	//m_simulator->T(itr->second.model_id, _transform_org); // 초기 위치 재적용

	DWORD * pMovableBoxPtr = (DWORD*)data;

	rxStaticBody * pBox = (rxStaticBody*) *pMovableBoxPtr;

	// pos of model + delt of pos
	if (pBox != 0) {
		HTransform T0 = pBox->T();
		HTransform T1 = T0;

		T1.r[1] += MOVE_DELTA;
		pBox->T(T1);
	}
}

void OnKeyBack(void* data)
{
	//HTransform T0(Rotation(), Displacement(0.f, 0.f, 0.2f));
	//m_simulator->T(itr->second.model_id, T0);
	//m_simulator->T(itr->second.model_id, _transform_org); // 초기 위치 재적용

	DWORD * pMovableBoxPtr = (DWORD*)data;

	rxStaticBody * pBox = (rxStaticBody*) *pMovableBoxPtr;

	// pos of model + delt of pos
	if (pBox != 0) {
		HTransform T0 = pBox->T();
		HTransform T1 = T0;

		T1.r[1] -= MOVE_DELTA;
		pBox->T(T1);
	}
}


void Simulator::SetKeyboardHandler()
{
	/*PHYSICS_WORLD->addKeyboardEvent(VK_LEFT, OnKeyLeft, pMovableBox);
	PHYSICS_WORLD->addKeyboardEvent(VK_RIGHT, OnKeyRight, pMovableBox;
	PHYSICS_WORLD->addKeyboardEvent(VK_UP, OnKeyFront, pMovableBox);
	PHYSICS_WORLD->addKeyboardEvent(VK_DOWN, OnKeyBack, pMovableBox);*/

	PHYSICS_WORLD->addKeyboardEvent(VK_LEFT, OnKeyLeft, (void*)&_movableBoxPtr);
	PHYSICS_WORLD->addKeyboardEvent(VK_RIGHT, OnKeyRight, (void*)&_movableBoxPtr);
	PHYSICS_WORLD->addKeyboardEvent(VK_UP, OnKeyFront, (void*)&_movableBoxPtr);
	PHYSICS_WORLD->addKeyboardEvent(VK_DOWN, OnKeyBack, (void*)&_movableBoxPtr);
}


// 강원대
void Simulator::moveUp()
{
	void* data=(void*)&_movableBoxPtr;
	DWORD * pMovableBoxPtr = (DWORD*)data;

	rxStaticBody * pBox = (rxStaticBody*) *pMovableBoxPtr;

	// pos of model + delt of pos
	if (pBox != 0) {
		HTransform T0 = pBox->T();
		HTransform T1 = T0;

		T1.r[1] += 1;
		pBox->T(T1);
	}

}
void Simulator::moveDown()
{
	void* data=(void*)&_movableBoxPtr;
	DWORD * pMovableBoxPtr = (DWORD*)data;

	rxStaticBody * pBox = (rxStaticBody*) *pMovableBoxPtr;

	// pos of model + delt of pos
	if (pBox != 0) {
		HTransform T0 = pBox->T();
		HTransform T1 = T0;

		T1.r[1] -= 1;
		pBox->T(T1);
	}	

}


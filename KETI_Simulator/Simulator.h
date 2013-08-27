#pragma once
#include "rxSimulationSDK/rxSimulationSDK.h"

class Simulator 
{
public:
	Simulator();
	~Simulator();
	
	bool loadModel(const string_type& filename, const string_type& modelname, const HTransform& T0);
	bool unloadModel(rID id);
	
	bool unloadEnv(rID id);

	rID		loadEnvironment(const string_type& filename, const string_type& modelname, const HTransform& T0);
	rID		loadSystem(const string_type& filename, const string_type& modelname, const HTransform& T0);

	size_t	system_size()	{	return	PHYSICS_WORLD->getSystems().size();	}
	size_t	env_size()	{	return	PHYSICS_WORLD->getEnvironments().size();	}


	void setPeriod(rTime period);
	bool update();
	void reset();	
	
	SystemList& getSystems();
	EnvironmentList & getEnvironments();

	rxSystem* findSystem(rID id);
	void activate();
	void deactivate();

	void	clearDisplay();
	void	T(rID id, const HTransform & T0);
	void	initialize();

	double	getTime();

	// °­¿ø´ë
	void	moveUp();
	void	moveDown();
	
private:
	rTime _period;
	bool _bRun;
	bool _bStarted;


	DWORD	_movableBoxPtr;	//	for box control

	void	SetKeyboardHandler();
	
};
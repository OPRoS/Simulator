#ifndef __SIMULATIONCONTROLLER_H__
#define __SIMULATIONCONTROLLER_H__

#include "kai/kai.h"

class Simulator;

class SimulationController : public kaiServer
{
public:
	SimulationController();
	~SimulationController();

	void registerSimulator(Simulator* simulator);


protected:
	
	virtual void onMessage(int id, kaiMsg& msg);
	virtual void onAccept(int id);
	virtual void onClose(int id);

private:
	Simulator* _simulator;


};




#endif
#pragma once

#include "kai/kai.h"

class Simulator;

class RobotController : public kaiServer 
{
public:
	RobotController();
	~RobotController();
	
	void registerSimulator(Simulator* simulator);


	rxDevice* findDevice(rID id);

protected:
	virtual void onMessage(int id, kaiMsg& msg);
	virtual void onClose(int id);
	virtual void onAccept(int id);
private:
	
	Simulator* _simulator;
	float buffer[1000];
	char msgbuffer[4096];

};
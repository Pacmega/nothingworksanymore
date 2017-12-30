#ifndef DOOR_H_
#define DOOR_H_

#include "lib/enums.h"
#include "CommunicationHandler.h"
#include "TrafficLight.h"
#include "ValveRow.h"

class Door
{
public:
	Door(CommunicationHandler existingHandler, DoorType Type, DoorSide Side, MotorType Motor);
	~Door();
	
	void interruptReaction();
	int allowExit();
	int allowEntry();
	int openDoor();
	int closeDoor();
	int stopDoor();

private:
	bool messageReceived;
	bool interruptCaught;
	CommunicationHandler cHandler;
	DoorType type;
	DoorSide side;
	MotorType motor;
	TrafficLight lightInside;
	TrafficLight lightOutside;
	ValveRow topValves;
	ValveRow middleValves;
	ValveRow bottomValves;
};

#endif
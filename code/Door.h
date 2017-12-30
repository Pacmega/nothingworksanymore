#ifndef DOOR_H_
#define DOOR_H_

#include "lib/enums.h"
#include "CommunicationHandler.h"
#include "TrafficLight.h"
#include "ValveRow.h"

class Door
{
private:
	bool messageReceived;
	bool interruptCaught;
	CommunicationHandler cHandler;
	DoorType type;
	DoorSide side;	

public:
	Door(CommunicationHandler existingHandler, DoorType Type, DoorSide Side);
	~Door();
	
	void interruptReaction();
	int allowExit();
	int allowEntry();
	int openDoor();
	int closeDoor();
	int stopDoor();

	TrafficLight lightInside;
	TrafficLight lightOutside;
	ValveRow topValves;
	ValveRow middleValves;
	ValveRow bottomValves;
};

#endif
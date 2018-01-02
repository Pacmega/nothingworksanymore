#ifndef COMMUNICATIONHANDLER_H_
#define COMMUNICATIONHANDLER_H_

#include "SimulationCommunicator.h"
#include "lib/enums.h"



class CommunicationHandler
{
public:
	CommunicationHandler(int socket);
	~CommunicationHandler();

	DoorState getDoorState(DoorSide side);
	bool lockDoor(DoorSide side);
	bool unlockDoor(DoorSide side);
	bool openDoor(DoorSide side);
	bool closeDoor(DoorSide side);
	bool stopDoor(DoorSide side);
	bool getValveOpened(DoorSide side, int row);
	bool valveOpen(DoorSide side, int row);
	bool valveClose(DoorSide side, int row);
	int redLight(int lightLocation);
	int greenLight(int lightLocation);
	LightState getLightState(int lightLocation);
	WaterLevel getWaterLevel();
	
private:
	SimulationCommunicator simulation;
	char* receivedMessage;
};

#endif
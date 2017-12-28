#ifndef COMMUNICATIONHANDLER_H_
#define COMMUNICATIONHANDLER_H_

#include "SimulationCommunicator.h"
#include "lib/enums.h"

class CommunicationHandler
{
public:
	CommunicationHandler(int socket);
	~CommunicationHandler();

	bool sendMsgAck(const char messageToSend[]);

	DoorState getDoorState(DoorSide side);
	bool getDoorValveOpened(DoorSide side, int row);
	
	int redLight(int lightLocation);
	int greenLight(int lightLocation);
	LightState getLightState(int lightLocation);
	
	WaterLevel getWaterLevel();
	
private:
	SimulationCommunicator simulation;
};

#endif
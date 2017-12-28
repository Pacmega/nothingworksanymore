#ifndef COMMUNICATIONHANDLER_H_
#define COMMUNICATIONHANDLER_H_ value

#include "NetworkInterface.h"
#include "lib/enums.h"

class CommunicationHandler
{
public:
	CommunicationHandler(int socket);
	~CommunicationHandler();

	bool sendMsgAck(char* messageToSend);
	bool getDoorValveOpened(DoorSide side, int row);
	WaterLevel getWaterLevel(char* messageToSend);
	DoorState getDoorState(char* messageToSend);
	LightState getLightState(char* messageToSend);
	
	
private:
	NetworkInterface interface;
};

#endif
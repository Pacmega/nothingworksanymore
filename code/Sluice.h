#ifndef SLUICE_H_
#define SLUICE_H_

#include "lib/enums.h"
#include "CommunicationHandler.h"
#include "Door.h"

class Sluice
{
public:
	Sluice(int port);
	~Sluice();
	
	int start();
	int allowEntry();
    int allowExit();
    void passInterrupt();
    CommunicationHandler cHandler;

private:
	Door leftDoor;
	Door rightDoor;

	bool emergency;
	SluiceState stateBeforeEmergency;

	int sluiceUp(WaterLevel currentWLevel); // TODO: add this function to the class diagram
	int sluiceDown(WaterLevel currentWLevel); // TODO: add this function to the class diagram
	bool closeValves(DoorSide side);
};

#endif